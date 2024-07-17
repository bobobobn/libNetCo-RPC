#include <iostream>
#include <string>
#include <sys/sysinfo.h>

#include "../include/log.h"
#include "../include/rpc_proto/rpc_client.h"
#include "../include/rpc_proto/int_message.pb.h"
#include "../include/rpc_proto/rpc_response_header.pb.h"
#include "../include/zk_client.h"
#include "../include/parameter.h"


static int64_t getRand(int64_t n){
        std::random_device rd;                             // 生成随机数种子
        std::mt19937 gen(rd());                            // 定义随机数生成引擎
        std::uniform_int_distribution<int64_t> distrib_int(1, n); // 定义随机数分布，生成在[1,n]之间的的均匀分布整数
        return distrib_int(gen);
}
void rpc_client_worker(netco::RpcClient& rpc_client, int loop_time)
{
    IntMessage int_message;
    int_message.set_value(10);
    std::string buf;
    int_message.SerializeToString(&buf);
    NETCO_LOG()<<"client call factorial method";
    std::string result;
    RpcResponseHeader header;
    for(int i=0;i<loop_time;i++){
        rpc_client.call("test", "factorial", buf, result, header);
        IntMessage int_result;
        int_result.ParseFromString(result);
        NETCO_LOG_FMT_INFO(NETCO_LOG_ROOT(), "client recv factorial result: %d",int_result.value());
        NETCO_LOG_FMT("the %d loop", i);
        netco::co_sleep(1000);
    }
}


int main()
{
    auto dice = getRand(1008680231);
    NETCO_LOG()<<("test: dice: %d", dice);
    NETCO_LOG()<<("test: add one rpc client");
    //TcpClient tcp_client_test;
    netco::RpcClient rpc_client_test;
    int loop_time = 10;
    for(int i=0;i<100;i++){
        netco::co_go([&rpc_client_test,&loop_time](){
            rpc_client_worker(rpc_client_test,loop_time);
        });
    }
    
    netco::sche_join();
    return 0;
}