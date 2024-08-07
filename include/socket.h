#pragma once

#include "utils.h"
#include "parameter.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>

#include "../include/log.h"

struct tcp_info;

namespace netco 
{
	/** hook socket*/
	class Socket
	{
	public:
		using Ptr = std::shared_ptr<Socket>;
		explicit Socket(int sockfd, std::string ip = "", int port = -1)
			: _sockfd(sockfd), _port(port), _ip(std::move(ip))
		{
			if (sockfd > 0)
			{
				/** socket为非阻塞*/
				setNonBolckSocket();		
			}
		}

		Socket(std::string type = "TCP")
			: _port(-1), _ip("")
		{ 
			if(type == "UDP" || type == "udp")
			{
				_sockfd = ::socket(AF_INET,SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_UDP);
			}
			else if(type == "TCP" || type == "tcp")
			{
				_sockfd = ::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,IPPROTO_TCP);
			}
			else
			{
				NETCO_LOG()<<("Socket type set error,default set the Socket type to TCP");
				_sockfd = ::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,IPPROTO_TCP);
			}
		}

		~Socket();

		/** 返回当前Socket对应的系统fd*/
		int fd() const { return _sockfd; }

		/** 返回当前Socket是否可用*/
		bool isUseful() { return _sockfd >= 0; }

		/** 绑定ip和port到当前Socket*/
		int bind(const char* ip,int port);

		/** 监听当前Socket*/
		int listen();

		/** 监听Socket接收一个连接，并返回一个新的Socket连接*/
		Ptr accept();

		/** 协程化改造-从Socket中读取数据*/
		ssize_t read(void* buf, size_t count);

		/** 当前Socket连接到指定地址*/
		int connect(const char* ip, int port);

		/** 协程化改造-当前Socket发送数据*/
		ssize_t send(const void* buf, size_t count);

		/** 返回ip和port*/
		std::string ip() { return _ip; }
		int port() { return _port; }

		/** 获得Socket的配置选项*/
		bool getSocketOpt(struct tcp_info*) const;
		bool getSocketOptString(char* buf, int len) const;
		std::string getSocketOptString() const;

		/** 关闭写操作*/
		int shutdownWrite();

		/** 设置是否开启Nagle算法降低包延迟*/
		int setTcpNoDelay(bool on);

		/** 设置地址复用*/
		int setReuseAddr(bool on);

		/** 设置端口复用*/
		int setReusePort(bool on);

		/** 设置心跳检测*/
		int setKeepAlive(bool on);

		/** 设置套接字为非阻塞*/
		int setNonBolckSocket();

		/** 设置套接字为阻塞*/
		int setBlockSocket();

		// TODO: HOOK UDP
		ssize_t recvfrom(int sockfd, void* buf, int len, unsigned int flags,
						sockaddr* from, socklen_t* fromlen);
						
		ssize_t sendto(int sockfd, const void* buf, int len, unsigned int flags,
						const struct sockaddr* to, int tolen);
	private:

		/** 接收一个连接，并返回一个新的Socket连接的具体实现*/
		Ptr accept_raw();

		/** 系统套接字*/
		int _sockfd;

		/** 引用计数*/
		int* _pRef;

		/** 端口号*/
		int _port;

		/** IP地址*/
		std::string _ip;
	};

}
