#include <iostream>
#include <string>
#include <sys/sysinfo.h>

#include "../include/log.h"
#include "../include/rpc_proto/rpc_client.h"
#include "../include/rpc_proto/int_message.pb.h"
#include "../include/rpc_proto/rpc_response_header.pb.h"
#include "../include/zk_client.h"
#include "../include/parameter.h"



void rpc_client_worker(netco::RpcClient& rpc_client, int loop_time)
{
    IntMessage int_message;
    int_message.set_value(10);
    std::string buf;
    int_message.SerializeToString(&buf);
    NETCO_LOG()<<"client call factorial method";
    std::string result;
    RpcResponseHeader header;
    rpc_client.call("test", "factorial", buf, result, header);
    IntMessage int_result;
    int_result.ParseFromString(result);
    NETCO_LOG()<<("client recv factorial result: %d",int_result.value());
}

int main()
{
    NETCO_LOG()<<("test: add one rpc client");
    //TcpClient tcp_client_test;
    netco::RpcClient rpc_client_test;
    int loop_time = 100;
	netco::co_go([&rpc_client_test,&loop_time](){
		rpc_client_worker(rpc_client_test,loop_time);
	});
    netco::sche_join();
    return 0;
}