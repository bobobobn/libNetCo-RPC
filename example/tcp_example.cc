#include "../include/netco_api.h"
#include "../include/tcpserver.h"
#include <iostream>

int main()
{
    using namespace netco;
    netco::TcpServer tcpSrv;
    tcpSrv.startServer(nullptr, 9989);
    netco::sche_join();    
    std::cout<<"end"<<std::endl;
}