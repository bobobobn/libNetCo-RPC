#include <iostream>
#include <string>
#include <sys/sysinfo.h>

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


int main()
{
    //TcpClient tcp_client_test;
    // NETCO_LOG_ROOT()->setLevel(netco::LogLevel::FATAL);
    int loop_time = 10;
    for(int i=0;i<1000;i++){
        netco::co_go(test);
    }
    
    netco::sche_join();
    return 0;
}