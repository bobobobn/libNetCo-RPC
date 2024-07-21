#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/sysinfo.h>

#include "../include/log.h"
#include "../include/rpc_proto/rpc_server.h"
#include "../include/rpc_proto/method_callback_example.h"
#include "../include/rpc_proto/zk_name_service_register.h"

using namespace netco;

static int delay = 1;
TcpServer::conn_callback set_delay(
    [](netco::Socket::Ptr connect_socket)
    {
        NETCO_LOG()<<("add one client connection");
        
        /** 设置数据缓冲区*/
        std::vector<char> buf;
        buf.resize(2048);
        while(true)
        {
            NETCO_LOG()<<("--------start one read-write process loop------------");
            /** readnum是实际上读到的数据*/
            auto readNum = connect_socket->read((void*)&(buf[0]), buf.size());	//这里也会发生协程切换(切回去是运行loop)
            //std::string ok = "HTTP/1.0 200 OK\r\nServer: netco/0.1.0\r\nContent-Type: text/html\r\n\r\n";
            if(readNum <= 0)
            {
                /** read = 0 exit*/
                break;
            }
            int new_delay = atoi(buf.data());
            std::cout << "new_delay: " << new_delay << std::endl;
			delay = new_delay;
			static const char * msg = "1";
			connect_socket->send(msg, strlen(msg));
            /** 设置延迟*/
        }
    }
);

void spin(int delay/*ms*/){
	std::cout << "spin " << delay << "ms" << std::endl;
	netco::co_sleep(delay);
}
std::string factorial_delay(const std::string& msg){
	spin(delay);
	return factorial(msg);
}
int main(int argc, char* argv[])
{
	/** test the tcp client and server  result : success*/

	/** test the rpc client and server*/
	if(argc!= 2)
	{
		std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
		return 1;
	}
	int port = std::stoi(argv[1]);
	NETCO_LOG_ROOT()->setLevel(netco::LogLevel::ERROR);
	netco::TcpServer s;
	s.register_connection(set_delay);
	s.start(nullptr,port-1000);
	netco::RpcServer rpc_server;
	rpc_server.add_service("test", "factorial", &factorial_delay);	
	rpc_server.set_name_service_register( netco::ZkNameServiceRegister::New() );
	rpc_server.start(nullptr, port);
	netco::sche_join(); 
	std::cout << "end" << std::endl;
	return 0;
}
