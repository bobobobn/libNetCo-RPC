#ifndef LOCALITY_AWARE_LOAD_BALANCER_H_
#define LOCALITY_AWARE_LOAD_BALANCER_H_
#include <random>
#include <unordered_map>
#include <vector>
#include <atomic>
#include "load_balancer.h"
#include "../container/doubly_buffered_data.h"
#include "../container/bounded_queue.h"
#include "socket_channel.h"
#include "../utils.h"


namespace netco{
    using ServerName = std::string;
    /**
     * @brief Locality Aware Load Balancer, a automatically adjusted weighted random load balancer based on QPS and latency.
     * details refer to https://github.com/apache/brpc/blob/master/docs/cn/lalb.md
     * 注意到：权重值几乎是一直在变化(频繁写)，所以我们使用原子操作来更新权重(提高并行度)，导致total_weight / node_weight/ parent weight 可能不一致但最终一致，查找算法通过[1.查找符合的节点 2.更新结点权值 3.必要时重新查找]适应这一点。树的结构很少改变但读频繁，所以我们使用DoublyBufferedData来保证并发安全，同时支持读写分离，提高性能。
     * 查找：基于完全树实现，每个节点保存了左子树的权重值之和，以及当前节点的权重值。相当于在一个前缀和数组二分查找
     * 添加：基于DoublyBufferedData实现，添加新节点到完全树末尾，向上更新权重值之和
     * 删除：基于DoublyBufferedData实现，将目标节点的字段值与末尾结点交换，然后删除末尾结点，向上更新权重值之和
     * server_map保留了server_name与weight_tree的下标映射关系
     */
    class LocalityAwareLoadBalancer : public LoadBalancer {
        public:
            LocalityAwareLoadBalancer(SocketChannel::NewFn channel_factory) : _channel_factory(channel_factory), server_map_(), total_weight_(0) { }
            virtual ~LocalityAwareLoadBalancer() = default;
            void add_server(const ServerName& server) override;
            void remove_server(const ServerName& server) override;
            int select_server(const SelectIn& in, SelectOut* out) override;
            size_t AddServersInBatch(const std::vector<ServerName>& servers) override;
            size_t RemoveServersInBatch(const std::vector<ServerName>& servers) override;
            void Feedback(const CallInfo& info) override;
            static LoadBalancer::UniquePtr New(SocketChannel::NewFn channel_factory) {
                return std::unique_ptr<LocalityAwareLoadBalancer>(new LocalityAwareLoadBalancer(channel_factory));
            }

        private:
            SocketChannel::NewFn _channel_factory;
            struct TimeInfo {
                int64_t latency_sum;         // microseconds
                int64_t end_time_us;
            };
            class Weight{
            public:
                static const int64_t RECV_QUEUE_SIZE = 128;
                struct AddInflightResult
                {
                    bool chosen;
                    int64_t weight_diff;
                };
                explicit Weight(int64_t initial_weight) :
                _weight(initial_weight)
                , _base_weight(initial_weight)
                , _begin_time_sum(0)
                , _begin_time_count(0)
                , _old_diff_sum(0)
                , _old_index((size_t)-1L)
                , _old_weight(0)
                , _avg_latency(0)
                , _time_q(_time_q_items, sizeof(_time_q_items), netco::NOT_OWN_STORAGE)
                , last_time_us(utils::gettimeofday_us())
                { }
                // Called in Feedback() to recalculate _weight.
                // Returns diff of _weight.
                int64_t Update(const CallInfo&, size_t index);
                // call ResetWeight to update the current node, and return a struct 
                // containing {boolean chosen, int64_t weight_diff}
                // chosen: whether the node is still satisfied to the dice
                // weight_diff: the diff of weight of the current node and its parent
                AddInflightResult AddInflight(const SelectIn& in, size_t index, int64_t dice);
                // Reset weight based on current time, and return the diff of weight
                int64_t ResetWeight(size_t index, int64_t now_us);
                // Set weight_base to avg_weight, and return the diff of weight
                int64_t MarkFailed(size_t index, int64_t avg_weight);
                // Mark where the index of this node is going to be used in the future, and return the weight before
                int64_t MarkOld(size_t index);
                std::pair<int64_t, int64_t> ClearOld();
                // Set weight_base to 0, and return the weight before
                int64_t Disable();
                bool Disabled() const { 
                    return _base_weight < 0;
                }
                // Weight of self. Notice that this value may change at any time.
                int64_t volatile_value() const { return _weight; }

            private:
                int64_t last_time_us;
                // base_weight = QPS * WEIGHT_SCALE / latency ^ p
                int64_t _base_weight;
                // weight = base_weight * avg_latency / inflight_delay
                int64_t _weight;
                int64_t _avg_latency;
                // inflight_delay = current_time - begin_time_sum / begin_time_count
                int64_t _begin_time_sum;
                int _begin_time_count;
                // 用于foreground和background的同步
                int64_t _old_diff_sum;
                size_t _old_index;
                int64_t _old_weight;
                // // 采样窗口，计算延时和QPS, TODO: BoundedQueue
                BoundedQueue<TimeInfo> _time_q;    
                TimeInfo _time_q_items[RECV_QUEUE_SIZE];  
                MutexLock _mutex;  
            };

            struct ServerInfo {
                using WeightPtr = std::shared_ptr<Weight>;
                using AtomicIntPtr = std::shared_ptr<std::atomic<int64_t>>;
                ServerName server_name;         // store the IpPort of server
                AtomicIntPtr left;     // store the weight sum of the left subtree 
                WeightPtr weight;                 // store the weight of the current node

                ServerInfo(const ServerName& server_name, const int64_t& initial_weight) : server_name(server_name), left(new std::atomic<int64_t>(0)), weight(new Weight(initial_weight)) { }
            };

            class Servers{
            public:
                std::vector<ServerInfo> weight_tree;
                std::unordered_map<ServerName, size_t> server_map;
                // Add diff to left_weight of all parent nodes of node `index'.
                // Not require position `index' to exist.
                void UpdateParentWeight(int64_t diff, size_t index) const {
                    while (index != 0) {
                        const size_t parent_index = (index - 1) >> 1;
                        if ((parent_index << 1) + 1 == index) {  // left child
                            weight_tree[parent_index].left->fetch_add(
                                diff, std::memory_order_relaxed);
                        }
                        index = parent_index;
                    }
                }
            };

            static size_t Add(Servers& bg, const Servers& fg , const ServerName& server_name, LocalityAwareLoadBalancer* balancer);
            static size_t Remove(Servers& servers, const ServerName& server_name, LocalityAwareLoadBalancer* balancer);

            DoublyBufferedData<Servers> servers_dbd_;
            // 在删除或添加时，优先查询是否存在，避免竞争
            std::unordered_map<ServerName, size_t> server_map_;
            MutexLock server_map_mutex_;
            std::atomic<int64_t> total_weight_;
    };
}
#endif