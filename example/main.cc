//@author Liu Yukang
#include <iostream>
#include <sys/sysinfo.h>

#include "../include/processor.h"
#include "../include/netco_api.h"
#include "../include/socket.h"
#include "../include/mutex.h"

using namespace netco;

NetCo* myNetCo = NetCo::getNetCo(::get_nprocs_conf());
void single_acceptor_server_test()
{
	
	myNetCo->co_go(
		[]{
			netco::Socket listener;
			if (listener.isUseful())
			{
				listener.setTcpNoDelay(true);
				listener.setReuseAddr(true);
				listener.setReusePort(true);
				if (listener.bind(8099) < 0)
				{
					return;
				}
				listener.listen();
			}
			while (1){
				netco::SocketPtr conn = listener.accept();
				conn->setTcpNoDelay(true);
				myNetCo->co_go(
					[conn]
					{
						std::vector<char> buf;
						buf.resize(2048);
						while (1)
						{
							auto readNum = conn->read((void*)&(buf[0]), buf.size());
							std::string ok = "HTTP/1.0 200 OK\r\nServer: netco/0.1.0\r\nContent-Type: text/html\r\n\r\n";
							if(readNum < 0){
								break;
							}
							conn->send(ok.c_str(), ok.size());
							conn->send((void*)&(buf[0]), readNum);
							if(readNum < (int)buf.size()){
								break;
							}
						}
						myNetCo->co_sleep(100);//��Ҫ��һ�£�����û������Ͼ͹ر���
    					std::cout<<"down here"<<std::endl;
					}
					);
			}
		}
	);
}

void test_co()
{
	
	myNetCo->co_go(
		[]{
			std::cout<<"co func"<<std::endl;
			myNetCo->co_sleep(100);
		}
	);
}


int main()
{
	

	single_acceptor_server_test();
	// test_co();
	myNetCo->sche_join();
	std::cout << "end" << std::endl;
	return 0;
}
