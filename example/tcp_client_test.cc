#include <iostream>
#include <string>
#include <sys/sysinfo.h>

#include "../include/log.h"
#include "../include/tcp/tcp_client.h"


void tcp_client_worker(TcpClient& tcp_client,int& loop_time)
{
    tcp_client.connect("127.0.0.1",12345);
    char buf[2048];

    for(int i = 0; i < loop_time; ++i)
    {
        tcp_client.send("ping",4);
        tcp_client.recv(buf,2048);
        NETCO_LOG()<<("client %dth recv %s",i,buf);
    }
}

int main()
{
    NETCO_LOG()<<("---------------");
    NETCO_LOG()<<("TEST TCP CLIENT");
    NETCO_LOG()<<("---------------");

    // Default: ping-pong
    TcpClient tcp_client;
    int loop_time  = 10;
    netco::co_go([&tcp_client,&loop_time](){
		tcp_client_worker(tcp_client,loop_time);
	});
    netco::sche_join();
    return 0;
}