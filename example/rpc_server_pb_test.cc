#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/sysinfo.h>

#include "../include/log.h"
#include "../include/rpc_proto/rpc_server.h"
#include "../include/rpc_proto/method_callback_example.h"

/**
 * need server-stub and client-stub
*/
int main()
{
    NETCO_LOG()<<("test: start the server");
	/** test the tcp client and server  result : success*/
	// TcpServer s;
	// s.start(nullptr,12345);

	/** test the rpc client and server*/
	netco::RpcServer rpc_server;
	rpc_server.add_service("test", "factorial", &netco::factorial);	
	rpc_server.start(nullptr, 12345);
	netco::sche_join(); 
	std::cout << "end" << std::endl;
	return 0;
}
