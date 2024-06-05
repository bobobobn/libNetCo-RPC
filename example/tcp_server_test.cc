#include <iostream>
#include <string>
#include <sys/sysinfo.h>

#include "../include/log.h"
#include "../include/tcp/tcp_server.h"


int main()
{
    NETCO_LOG()<<("---------------");
    NETCO_LOG()<<("TEST TCP SERVER");
    NETCO_LOG()<<("---------------");

    // Default: ping-pong
    TcpServer tcp_server;
    tcp_server.start("127.0.0.1",8888);
    netco::sche_join();
    return 0;
}