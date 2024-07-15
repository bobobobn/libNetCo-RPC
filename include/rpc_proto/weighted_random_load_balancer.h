#pragma once
#include "load_balancer.h"
#include "name_service.h"
#include <random>


namespace netco{
    // get int random number in [1,n]
    static int getRand(int n){
        std::random_device rd;                             // 生成随机数种子
        std::mt19937 gen(rd());                            // 定义随机数生成引擎
        std::uniform_int_distribution<> distrib_int(1, n); // 定义随机数分布，生成在[1,n]之间的的均匀分布整数
        return distrib_int(gen);
    }
    class WeightedRandomLoadBalancer : public LoadBalancer<ServerNode> {
        public:
            void add_server(const vector<ServerNode>& servers) override;
            void remove_server(const vector<ServerNode>& servers) override;
            ServerNode* select_server(const vector<ServerNode>& servers) override;
    };
    ServerNode* WeightedRandomLoadBalancer::select_server(const vector<ServerNode>& servers){
        int n = servers.size();
        int weight_sum = 0;
        for(int i=0;i<n;i++){
            weight_sum += servers[i].weight;
        }
        int rand_num = getRand(weight_sum);
        int pre_sum = 0;
        /**
         * pre_sum: 前i个server的权重之和
         * rand_num: 随机数 ~ U(1, weight_sum)
         * 随机数落在(last_pre_sum, pre_sum]区间内，则选取该区间内的server
         * 权重为0的server不会被选取
         */
        for(int i=0;i<n;i++){
            pre_sum += servers[i].weight;
            if(rand_num <= pre_sum){
                return &servers[i];
            }
        }
    }
}