#pragma once
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <memory>

struct tcp_info;
namespace netco
{
    class Socket;
    using SocketPtr = typename std::shared_ptr<Socket>;
    class Socket
    {
    public:
        explicit Socket(int sockfd, std::string ip = "", int port = -1)
			: sockFd_(sockfd), port_(port), ip_(std::move(ip))
		{
			if (sockfd > 0)
			{
				setNonBlockSocket();
			}
		}
        Socket()
            :port_(-1), ip_("")
        {
            sockFd_= ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
        }
        ~Socket()
        {
            if(sockFd_>=0)
                close(sockFd_);
        }
        inline bool isUseful() { return sockFd_>=0; }

        void setNonBlockSocket();
        void setBlockSocket();
        int bind(int port);
        SocketPtr accept();
        int listen();
        ssize_t read(void* buf, size_t size);
        void connect(const char* ip, int port);
        ssize_t send(const void* buf, size_t size);
        bool getSocketOpt(struct tcp_info*) const;

		bool getSocketOptString(char* buf, int len) const;

		std::string getSocketOptString() const;

		int shutdownWrite();

		int setTcpNoDelay(bool on);

		int setReuseAddr(bool on);

		int setReusePort(bool on);

		int setKeepAlive(bool on);
    
    private:
        SocketPtr accept_raw();
    private:
        int sockFd_;
        int port_;
        std::string ip_;
    };
}