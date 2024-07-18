#include <iostream>
#include <string>
#include <sys/sysinfo.h>
#include <map>
#include <random>
#include "netco_api.h"
#include "short_socket_channel.h"


namespace netco{
    extern __thread int threadIdx;
}

void test()
{
    auto socket = netco::ShortSocketChannel::New("127.0.0.1:12345");
    if(socket->is_connected())
        std::cout<<"here is"<<netco::threadIdx<<std::endl;
    netco::co_sleep(10000);
}
static int64_t getRand(int64_t n){
        std::random_device rd;                             // 生成随机数种子
        std::mt19937 gen(rd());                            // 定义随机数生成引擎
        std::uniform_int_distribution<int64_t> distrib_int(1, n); // 定义随机数分布，生成在[1,n]之间的的均匀分布整数
        return distrib_int(gen);
}

int main()
{
    //TcpClient tcp_client_test;
    // NETCO_LOG_ROOT()->setLevel(netco::LogLevel::FATAL);
    std::map<int, int> m;
    for(int i = 0; i < 1000; i++){
        m[getRand(1000)]++;
    } 
    for(auto it = m.begin(); it != m.end(); it++){
        std::cout<<it->first<<" "<<it->second<<std::endl;
    }

    // int loop_time = 10;
    // for(int i=0;i<1000;i++){
    //     netco::co_go(test);
    // }
    
    // netco::sche_join();
    return 0;
}