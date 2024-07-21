#include <iostream>
#include <string>
#include <sys/sysinfo.h>

#include "../include/log.h"
#include "../include/rpc_proto/rpc_client.h"
#include "../include/rpc_proto/int_message.pb.h"
#include "../include/rpc_proto/rpc_response_header.pb.h"
#include "../include/zk_client.h"
#include "../include/parameter.h"


static const int LOOP_TIME = 15;
static int64_t getRand(int64_t n){
        std::random_device rd;                             // 生成随机数种子
        std::mt19937 gen(rd());                            // 定义随机数生成引擎
        std::uniform_int_distribution<int64_t> distrib_int(1, n); // 定义随机数分布，生成在[1,n]之间的的均匀分布整数
        return distrib_int(gen);
}
__thread int64_t success_count = 0;
__thread double delay_count = 0;
int success_max[4];
double delay_max[4];
void rpc_client_worker(netco::RpcClient& rpc_client, int64_t start_time)
{    
    IntMessage int_message;
    int_message.set_value(10);
    std::string buf;
    int_message.SerializeToString(&buf);
    NETCO_LOG()<<"client call factorial method";
    std::string result;
    RpcResponseHeader header;
    while(1){
        int64_t start = netco::utils::gettimeofday_us();
        rpc_client.call("test", "factorial", buf, result, header);
        IntMessage int_result;
        int_result.ParseFromString(result);
        NETCO_LOG_FMT_INFO(NETCO_LOG_ROOT(), "client recv factorial result: %d",int_result.value());
        delay_count += (netco::utils::gettimeofday_us() - start)*0.001;
        success_count++;
        if(netco::utils::gettimeofday_us() - start_time > LOOP_TIME*1000000){
            netco::Scheduler::getScheduler()->getProcessor(netco::threadIdx)->stop();
            break;
        }
    }
    // int coroutine_count = netco::Scheduler::getScheduler()->getProcessor(netco::threadIdx)->getCoCnt();
    // if(netco::Scheduler::getScheduler()->getProcessor(netco::threadIdx)->getCoCnt() == 1)
    if(success_max[netco::threadIdx] < success_count){
        success_max[netco::threadIdx] = success_count;
    }
    if( delay_max[netco::threadIdx] < delay_count){
        delay_max[netco::threadIdx] = delay_count;
    }
}


int main()
{
    auto dice = getRand(1008680231);
    NETCO_LOG_ROOT()->setLevel(netco::LogLevel::ERROR);
    NETCO_LOG()<<("test: dice: %d", dice);
    NETCO_LOG()<<("test: add one rpc client");
    //TcpClient tcp_client_test;
    netco::RpcClient rpc_client_test;
    int64_t start_time = netco::utils::gettimeofday_us();
    for(int i=0;i<5000;i++){
        netco::co_go([&rpc_client_test,&start_time](){
            rpc_client_worker(rpc_client_test,start_time);
        });
    }    
    netco::sche_join();
    int q_total = 0;
    uint64_t total_delay = 0;
    for (size_t i = 0; i < 4; i++)
    {
        q_total += success_max[i];
        total_delay += delay_max[i];
    }
    std::cout << "QPS: " << q_total/LOOP_TIME << " avg_delay: " << total_delay/q_total << " ms" <<std::endl;
    return 0;
}