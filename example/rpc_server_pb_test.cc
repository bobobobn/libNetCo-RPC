#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/sysinfo.h>

#include "../include/log.h"
#include "../include/rpc_proto/rpc_server.h"
#include "../include/rpc_proto/method_callback_example.h"
#include "../include/rpc_proto/zk_name_service_register.h"

/**
 * need server-stub and client-stub
*/
int main(int argc, char* argv[])
{
    NETCO_LOG()<<("test: start the server");
	/** test the tcp client and server  result : success*/
	// TcpServer s;
	// s.start(nullptr,12345);

	/** test the rpc client and server*/
	if(argc!= 2)
	{
		std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
		return 1;
	}
	int port = std::stoi(argv[1]);
	netco::RpcServer rpc_server;
	rpc_server.add_service("test", "factorial", &netco::factorial);	
	rpc_server.set_name_service_register( netco::ZkNameServiceRegister::New() );
	rpc_server.start(nullptr, port);
	netco::sche_join(); 
	std::cout << "end" << std::endl;
	return 0;
}
