#pragma once
#include <random>
#include <unordered_map>
#include <vector>
#include <atomic>
#include "load_balancer.h"
#include "../container/doubly_buffered_data.h"


namespace netco{
    static const int64_t DEFAULT_QPS = 1;
    static const size_t INITIAL_WEIGHT_TREE_SIZE = 128;
    // 1008680231, 放大系数，为了让整数存储权值
    static const int64_t WEIGHT_SCALE =
    std::numeric_limits<int64_t>::max() / 72000000 / (INITIAL_WEIGHT_TREE_SIZE - 1);

    // get int random number in [1,n]
    static int64_t getRand(int64_t n){
        std::random_device rd;                             // 生成随机数种子
        std::mt19937 gen(rd());                            // 定义随机数生成引擎
        std::uniform_int_distribution<int64_t> distrib_int(1, n); // 定义随机数分布，生成在[1,n]之间的的均匀分布整数
        return distrib_int(gen);
    }

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
    class LocalityAwareLoadBalancer : public LoadBalancer<ServerName> {
        public:
            void add_server(const ServerName& server) override;
            void remove_server(const ServerName& server) override;
            int select_server(const SelectIn& in, SelectOut* out) override;
        
        private:

            class Weight{
            public:
                struct AddInflightResult
                {
                    bool chosen;
                    int64_t weight_diff;
                };
                // call ResetWeight to update the current node, and return a struct 
                // containing {boolean chosen, int64_t weight_diff}
                // chosen: whether the node is still satisfied to the dice
                // weight_diff: the diff of weight of the current node and its parent
                AddInflightResult AddInflight(const SelectIn& in, size_t index, int64_t dice);
                // Reset weight based on current time, and return the diff of weight
                int64_t ResetWeight(size_t index, int64_t now_us);
                // Set weight_base to avg_weight, and return the diff of weight
                int64_t MarkFailed(size_t index, int64_t avg_weight);
                // Set weight_base to 0, and return the weight before
                int64_t Disable() { 
                    MutexGuard lock(_mutex);
                    auto ret = _weight; 
                    _base_weight = -1, _weight = 0;
                    return ret;
                }
                // Weight of self. Notice that this value may change at any time.
                int64_t volatile_value() const { return _weight; }

            private:
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
                // BoundedQueue<TimeInfo> _time_q;      
                MutexLock _mutex;  
            };

            struct ServerInfo {
                using WeightPtr = std::shared_ptr<Weight>;
                using AtomicIntPtr = std::shared_ptr<std::atomic<int64_t>>;
                ServerName server_name;         // store the IpPort of server
                AtomicIntPtr left;     // store the weight sum of the left subtree 
                WeightPtr weight;                 // store the weight of the current node

                ServerInfo(const ServerName& server_name) : server_name(server_name), left(new std::atomic<int64_t>(0)), weight(new Weight()) { }
            };

            class Servers{
            public:
                std::vector<ServerInfo> weight_tree;
                std::unordered_map<ServerName, size_t> server_map;
                // Add diff to left_weight of all parent nodes of node `index'.
                // Not require position `index' to exist.
                void UpdateParentWeight(int64_t diff, size_t index);
            };

            static size_t Add(Servers& bg, const Servers& fg , const ServerName server_name, LocalityAwareLoadBalancer* balancer);
            static size_t Remove(Servers& servers, const ServerName& server_name, LocalityAwareLoadBalancer* balancer);

            DoublyBufferedData<Servers> servers_dbd_;
            // 在删除或添加时，优先查询是否存在，避免竞争
            std::unordered_map<ServerName, size_t> server_map_;
            MutexLock server_map_mutex_;
            std::atomic<int64_t> total_weight_;
    };
    size_t LocalityAwareLoadBalancer::Add(Servers& bg, const Servers& fg , const ServerName server_name, LocalityAwareLoadBalancer* balancer)
    {
        //TODO: ADD a NEW SERVER
        auto fg_it = fg.server_map.find(node.server_name);
        if(fg_it != fg.server_map.end()){
            // fg has it, add it to bg, only change the structure of bg, weight has been changed by fg
            bg.server_map[node.server_name] = fg.server_map[node.server_name];
            bg.weight_tree.push_back(fg.weight_tree[fg_it->second]);
        }
        else{
            // fg doesn't have it, add it to bg, need to create a new node
            size_t n = bg.weight_tree.size();
            ServerInfo node(server_name);
            bg.server_map[node.server_name] = n;
            bg.weight_tree.push_back(node);
            auto diff = node.weight->volatile_value();
            if(diff){
                bg.UpdateParentWeight(diff, n);
                balancer->total_weight_.fetch_add(diff, std::memory_order_relaxed);
            }
        }
        return 0;
    }

    size_t LocalityAwareLoadBalancer::Remove(Servers& bg, const Servers& fg, const ServerName& server_name, LocalityAwareLoadBalancer* balancer) {
        //TODO: REMOVE a EXISTING SERVER
        size_t index = bg.server_map[server_name];
        bg.server_map.erase(server_name);
        auto weight_ptr_to_delete = bg.weight_tree[index].weight;
        const auto weight_rm = weight_ptr_to_delete->Disable();
        if(index + 1 == bg.weight_tree.size()){
            // remove the last node
            bg.weight_tree.pop_back();
            if(weight_rm){
                bg.UpdateParentWeight(-weight_rm, index);
                balancer->total_weight_.fetch_add(-weight_rm, std::memory_order_relaxed);
            }
        }
        else{
            // swap the last node with the target node, and delete the last node
            auto weight_ptr_to_swap = bg.weight_tree.back().weight; // previously back()
            bg.weight_tree[index].weight = bg.weight_tree.back().weight;
            bg.weight_tree[index].server_name = bg.weight_tree.back().server_name;
            bg.server_map[bg.weight_tree.back().server_name] = index;
            bg.weight_tree.pop_back();
            
        }
         
    }

    void LocalityAwareLoadBalancer::add_server(const ServerName& server_name) {
        {
            MutexGuard lock(server_map_mutex_);
            if(++server_map_[server_map_] != 1){
                // server already exists
                return;
            }
        }
        servers_dbd_.ModifyWithForeground(Add, server_name, this);
    }
    
    void LocalityAwareLoadBalancer::remove_server(const ServerName& server_name) {
        {
            MutexGuard lock(server_map_mutex_);
            if(!server_map_.erase(server_name)){
                return;
            }
        }
        servers_dbd_.ModifyWithForeground(Remove, server_name, this);
    }
    
    int LocalityAwareLoadBalancer::select_server(const SelectIn& in, SelectOut* out) {
        auto servers_scope_ptr_ptr = servers_dbd_.GetDataPtr();
        auto& servers = **servers_scope_ptr_ptr;
        const size_t n = servers.weight_tree.size();
        size_t n_try = 0;
        int64_t total_weight = total_weight_.load(std::memory_order_relaxed);    
        int64_t dice = getRand(total_weight);
        int64_t self;
        
        int index = 0;
        while(total_weight > 0){
            const auto& node = servers.weight_tree[index];
            const int64_t left = node.left->load(std::memory_order_relaxed);
            if(dice < left){
                // dice落在左子树
                index = 2 * index + 1;
            }
            else if(dice >= left + (self = node.weight->volatile_value()))
            {
                // dice落在右子树
                index = 2 * index + 2;
                dice -= left + self;
                if(index < n){
                    continue;
                }
            }
            else if(1/*TODO: the server of this node is available*/){
                const auto result = node.weight->AddInflight(in, index, dice-left);
                if(result.weight_diff){
                    // 权值变化，更新父节点权值
                    servers.UpdateParentWeight(result.weight_diff, index);
                    total_weight_.fetch_add(result.weight_diff, std::memory_order_relaxed);
                }
                if(result.chosen){
                    out->need_feedback = true;
                    
                    return 0;
                }
                // 权值更新后不符合选择条件，重新搜索
                if(++n_try >= n){
                    break;
                }
            }
            // 节点服务器连接失败，标记为不可用
            // TODO: 标记为不可用后，需要更新父节点权值
            // TODO: 标记为不可用后，需要更新子节点权值
            else{
                auto diff = node.weight->MarkFailed(index, total_weight / n);
                if(diff){
                    servers.UpdateParentWeight(diff, index);
                    total_weight_.fetch_add(diff, std::memory_order_relaxed);
                }
                if(dice >= left + self + diff){
                    index = 2 * index + 2;
                    dice -= left + self + diff;                
                }
                else{
                    index = 2 * index + 1;
                    dice = getRand(left);
                }

                if(index < n){
                    continue;
                }
            }
            // 重新搜索
            if(++n_try >= n){
                break;
            }
            total_weight = total_weight_.load(std::memory_order_relaxed);
            dice = getRand(total_weight);
            index = 0;
        }
    }

}