#include "../../include/rpc_proto/locality_aware_load_balancer.h"
#include "log.h"
#include <fstream>
namespace netco{
    static const int64_t DEFAULT_QPS = 1;
    static const size_t INITIAL_WEIGHT_TREE_SIZE = 128;
    // 1008680231, 放大系数，为了让整数存储权值
    static const int64_t WEIGHT_SCALE =
    std::numeric_limits<int64_t>::max() / 72000000 / (INITIAL_WEIGHT_TREE_SIZE - 1);
    static const int64_t FLAGS_min_weight = 1000;
    static const int64_t FLAGS_punish_inflight_ratio = 1.5;
    static const int64_t FLAGS_punish_error_ratio = 1.2;

    // get int random number in [1,n]
    static int64_t getRand(int64_t n){
        std::random_device rd;                             // 生成随机数种子
        std::mt19937 gen(rd());                            // 定义随机数生成引擎
        std::uniform_int_distribution<int64_t> distrib_int(1, n); // 定义随机数分布，生成在[1,n]之间的的均匀分布整数
        return distrib_int(gen);
    }
    size_t LocalityAwareLoadBalancer::Add(Servers& bg, const Servers& fg , const ServerName& server_name, LocalityAwareLoadBalancer* balancer)
    {
        //TODO: ADD a NEW SERVER
        auto fg_it = fg.server_map.find(server_name);
        if(fg_it != fg.server_map.end()){
            // fg has it, add it to bg, only change the structure of bg, weight has been changed by fg
            bg.server_map[server_name] = fg_it->second;
            bg.weight_tree.push_back(fg.weight_tree[fg_it->second]);
        }
        else{
            // fg doesn't have it, add it to bg, need to create a new node
            size_t n = bg.weight_tree.size();
            int64_t inital_weight = WEIGHT_SCALE;
            if(n != 0){
                inital_weight = balancer->total_weight_.load(std::memory_order_relaxed) / n;
            }
            ServerInfo node(server_name, inital_weight);
            bg.server_map[server_name] = n;
            bg.weight_tree.push_back(node);
            auto diff = node.weight->volatile_value();
            if(diff){
                bg.UpdateParentWeight(diff, n);
                balancer->total_weight_.fetch_add(diff, std::memory_order_relaxed);
            }
        }
        return 0;
    }
    size_t LocalityAwareLoadBalancer::Remove(Servers& bg, const ServerName& server_name, LocalityAwareLoadBalancer* lb) {
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
                lb->total_weight_.fetch_add(-weight_rm, std::memory_order_relaxed);
            }
        }
        else{
            // swap the last node with the target node, and delete the last node
            auto weight_ptr_to_swap = bg.weight_tree.back().weight; // previously back()
            bg.weight_tree[index].weight = bg.weight_tree.back().weight;
            bg.weight_tree[index].server_name = bg.weight_tree.back().server_name;
            bg.server_map[bg.weight_tree.back().server_name] = index;
            bg.weight_tree.pop_back();
            if(weight_rm){
                // the first buffer
                // someone may be dealing with this node, which is still seen as back(), the traffic to this node will be saved in _old_diff_sum while calling ResetWeight.
                const int64_t add_weight = weight_ptr_to_swap->MarkOld(bg.weight_tree.size());
                const int64_t diff = add_weight - weight_rm;
                if (diff) {
                    bg.UpdateParentWeight(diff, index);
                    lb->total_weight_.fetch_add(diff, std::memory_order_relaxed);
                }
            }
            else{
                // the second buffer
                // Reset _old_* fields and get the weight change by SelectServer()
                // after MarkOld().
                const std::pair<int64_t, int64_t> p = weight_ptr_to_swap->ClearOld();
                // Add the diff to parent nodes of node `index'
                const int64_t diff = p.second;
                if (diff) {
                    bg.UpdateParentWeight(diff, index);
                }
                // Remove weight from parent nodes of last node.
                int64_t old_weight = - p.first - p.second;
                if (old_weight) {
                    bg.UpdateParentWeight(old_weight, bg.weight_tree.size());
                }
                lb->total_weight_.fetch_add(- p.first, std::memory_order_relaxed);
            }
        }
        return 0;                  
    }

    void LocalityAwareLoadBalancer::add_server(const ServerName& server_name) {
        {
            MutexGuard lock(server_map_mutex_);
            if(++server_map_[server_name] != 1){
                // server already exists
                return;
            }
        }
        servers_dbd_.ModifyWithForeground(Add, server_name, this);
    }
    size_t LocalityAwareLoadBalancer::AddServersInBatch(const std::vector<ServerName>& servers){
        //TODO: ADD a BATCH OF SERVERS
    }
    void LocalityAwareLoadBalancer::remove_server(const ServerName& server_name) {
        {
            MutexGuard lock(server_map_mutex_);
            if(!server_map_.erase(server_name)){
                return;
            }
        }
        servers_dbd_.Modify(Remove, server_name, this);
    }
    size_t LocalityAwareLoadBalancer::RemoveServersInBatch(const std::vector<ServerName>& servers){
        //TODO: REMOVE a BATCH OF SERVERS
    }
    int LocalityAwareLoadBalancer::select_server(const SelectIn& in, SelectOut* out) {
        auto servers_scope_ptr_ptr = servers_dbd_.GetDataPtr();
        auto& servers = **servers_scope_ptr_ptr;
        const size_t n = servers.weight_tree.size();
        size_t n_try = 0;
        int64_t total_weight = total_weight_.load(std::memory_order_relaxed);   
         
        int64_t dice = getRand(total_weight);
        NETCO_LOG_FMT("dice: %lld, total_weight: %lld", dice, total_weight);
        int64_t self = -1;
        int index = 0;
        while(total_weight > 0){
            const auto& node = servers.weight_tree[index];
            const int64_t left = node.left->load(std::memory_order_relaxed);
            self = node.weight->volatile_value();
            if(dice < left){
                // dice落在左子树
                index = 2 * index + 1;
                continue;
            }
            else if(dice >= left + self)
            {
                // dice落在右子树
                index = 2 * index + 2;
                dice -= left + self;
                if(index < n){
                    continue;
                }
            }
            else
            {
                const auto result = node.weight->AddInflight(in, index, dice-left);
                if(result.weight_diff){
                    // 权值变化，更新父节点权值
                    servers.UpdateParentWeight(result.weight_diff, index);
                    total_weight_.fetch_add(result.weight_diff, std::memory_order_relaxed);
                }
                if(result.chosen){
                    out->node = node.server_name;
                    out->need_feedback = true;                        
                    return 0;
                }
                // 权值更新后不符合选择条件，重新搜索
                if(++n_try >= n){
                    break;
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
        return -1;
    }

    inline LocalityAwareLoadBalancer::Weight::AddInflightResult LocalityAwareLoadBalancer::Weight::AddInflight(const SelectIn& in, size_t index, int64_t dice) {
        MutexGuard lock(_mutex);
        if (Disabled()) {
            AddInflightResult r = { false, 0 };
            return r;
        }
        const int64_t diff = ResetWeight(index, in.begin_time_us);
        if (_weight < dice) {
            // inflight delay makes the weight too small to choose.
            AddInflightResult r = { false, diff };
            return r;
        }
        _begin_time_sum += in.begin_time_us;
        ++_begin_time_count;
        AddInflightResult r = { true, diff };
        return r;
    }

    inline int64_t LocalityAwareLoadBalancer::Weight::ResetWeight(size_t index, int64_t now_us) {
        int64_t new_weight = _base_weight;
        if (_begin_time_count > 0) {
            const int64_t inflight_delay =
                now_us - _begin_time_sum / _begin_time_count;
            const int64_t punish_latency =
                (int64_t)(_avg_latency * FLAGS_punish_inflight_ratio);
            if (inflight_delay >= punish_latency && _avg_latency > 0) {
                new_weight = new_weight * punish_latency / inflight_delay;
            }
        }
        if (new_weight < FLAGS_min_weight) {
            new_weight = FLAGS_min_weight;
        }
        const int64_t old_weight = _weight;
        _weight = new_weight;
        const int64_t diff = new_weight - old_weight;
        if (_old_index == index && diff != 0) {
            _old_diff_sum += diff;
        }
        return diff;
    }

    int64_t LocalityAwareLoadBalancer::Weight::MarkFailed(size_t index, int64_t avg_weight) {
        MutexGuard lock(_mutex);
        if (_base_weight <= avg_weight) {
            return 0;
        }
        _base_weight = avg_weight;
        return ResetWeight(index, 0);
    }

    int64_t LocalityAwareLoadBalancer::Weight::MarkOld(size_t index){
        MutexGuard lock(_mutex);
        const int64_t saved = _weight;
        _old_weight = saved;
        _old_diff_sum = 0;
        _old_index = index;
        return saved;
    }
    std::pair<int64_t, int64_t> LocalityAwareLoadBalancer::Weight::ClearOld() {
        MutexGuard lock(_mutex);
        const int64_t old_weight = _old_weight;
        const int64_t diff = _old_diff_sum;
        _old_diff_sum = 0;
        _old_index = (size_t)-1;
        _old_weight = 0;
        return std::make_pair(old_weight, diff);
    }
    int64_t LocalityAwareLoadBalancer::Weight::Disable() {
        MutexGuard lock(_mutex);
        auto ret = _weight; 
        _base_weight = -1, _weight = 0;
        return ret;    
    }
    int64_t LocalityAwareLoadBalancer::Weight::Update(const CallInfo& ci, size_t index) {
        static MutexLock ofs_lock;        
        const int64_t end_time_us = utils::gettimeofday_us();
        const int64_t latency = end_time_us - ci.begin_time_us;
        MutexGuard lock(_mutex);
        if (Disabled()) {
            // The weight was disabled and will be removed soon, do nothing
            // and the diff is 0.
            return 0;
        }

        _begin_time_sum -= ci.begin_time_us;
        --_begin_time_count;

        if (latency <= 0) {
            // time skews, ignore the sample.
            return 0;
        }
        if (ci.error_code == 0) {
            // Add a new entry
            TimeInfo tm_info = { latency, end_time_us };
            if (!_time_q.empty()) {
                tm_info.latency_sum += _time_q.bottom()->latency_sum;
            }
            _time_q.elim_push(tm_info);
        } else {
            // TODO:出错情况的处理，以及(TODO:超时重试)
        }
        const int64_t top_time_us = _time_q.top()->end_time_us;
        const size_t n = _time_q.size();
        int64_t scaled_qps = DEFAULT_QPS * WEIGHT_SCALE;
        if (end_time_us > top_time_us) {        
            // Only calculate scaled_qps when the queue is full or the elapse
            // between bottom and top is reasonably large(so that error of the
            // calculated QPS is probably smaller).
            if (n == _time_q.capacity() ||
                end_time_us >= top_time_us + 1000000L/*1s*/) { 
                // will not overflow.
                scaled_qps = (n - 1) * 1000000L * WEIGHT_SCALE / (end_time_us - top_time_us);
                if (scaled_qps < WEIGHT_SCALE) {
                    scaled_qps = WEIGHT_SCALE;
                }
            }
            _avg_latency = (_time_q.bottom()->latency_sum -
                            _time_q.top()->latency_sum) / (n - 1);
        } else if (n == 1) {
            _avg_latency = _time_q.bottom()->latency_sum;
        } else {
            // end_time_us <= top_time_us && n > 1: the QPS is so high that
            // the time elapse between top and bottom is 0(possible in examples),
            // or time skews, we don't update the weight for safety.
            return 0;
        }
        if (_avg_latency == 0) {
            return 0;
        }
        _base_weight = scaled_qps / _avg_latency;
        // // 每秒输出权值信息
        // if( end_time_us - last_time_us > 1000000L/*1s*/){
        //     last_time_us = end_time_us;
        //     struct tm tm;
        //     time_t t;
        //     time(&t);
        //     localtime_r(&t, &tm);
        //     char buf[64];
        //     strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
        //     std::string weights;
        //     MutexGuard lock(ofs_lock);
        //     std::cout << "weight info: ["<<buf<<"]" << "node: " << index << ", weight: " << _base_weight << ", avg_latency: " << _avg_latency << " QPS: " << scaled_qps / WEIGHT_SCALE << "\n";
        // }
        return ResetWeight(index, end_time_us);
    }
    void LocalityAwareLoadBalancer::Feedback(const CallInfo& info){
        ServerName node_name = info.node;
        if(server_map_.find(node_name) == server_map_.end()){
            return;
        }
        {
            auto servers_scope_ptr_ptr = servers_dbd_.GetDataPtr();
            auto& servers = **servers_scope_ptr_ptr;
            // const类型的map，只能用find，不能用[]
            auto it = servers.server_map.find(node_name);
            const size_t index = it->second;
            auto weight_ptr = servers.weight_tree[index].weight;            
            int64_t diff = weight_ptr->Update(info, index);
            if (diff != 0) {
                servers.UpdateParentWeight(diff, index);
                total_weight_.fetch_add(diff, std::memory_order_relaxed);
            }
        }
    }
}