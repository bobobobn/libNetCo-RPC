#include "../include/socket.h"
#include "../include/parameter.h"
#include "../include/scheduler.h"
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/epoll.h>
using namespace netco;

void Socket::setNonBlockSocket()
{
    int oldOpt = fcntl(sockFd_, F_GETFL, 0);
    int ret = fcntl(sockFd_, F_SETFL, oldOpt | O_NONBLOCK);
}

void Socket::setBlockSocket()
{
    int oldOpt = fcntl(sockFd_, F_GETFL, 0);
    int ret = fcntl(sockFd_, F_SETFL, oldOpt & ~O_NONBLOCK);
}

SocketPtr Socket::accept_raw()
{
    int connfd = -1;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(sockaddr_in);
    connfd = ::accept(sockFd_, (sockaddr*) &client_addr, &addr_len);
    if(connfd<0)
        return std::make_shared<Socket>(connfd);
    
    int port = ntohs(client_addr.sin_port);          
	struct in_addr in = client_addr.sin_addr;
	char ip[INET_ADDRSTRLEN];   
	inet_ntop(AF_INET, &in, ip, sizeof(ip));
    return std::make_shared<Socket>(connfd, std::string(ip), port);
}

SocketPtr Socket::accept()
{
    SocketPtr connSock = accept_raw();
    if(connSock->isUseful())
        return connSock;
	netco::Scheduler::getScheduler(0)->getProcessor(threadIdx)->waitEvent(sockFd_, EPOLLIN | EPOLLPRI | EPOLLRDHUP | EPOLLHUP);
	auto con(accept_raw());
	if(con->isUseful()){
		return con;
	}
	return accept();   
    
}

int Socket::bind(int port)
{
    port_ = port;
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = ::bind(sockFd_, (struct sockaddr*) & serv, sizeof(serv));
	return ret;
}

int Socket::bind(const char* ip,int port)
{
    port_ = port;
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	if(ip == nullptr)
    {
        serv.sin_addr.s_addr = htonl(INADDR_ANY);   
    }
    else
    {
        serv.sin_addr.s_addr = inet_addr(ip);
    }
	int ret = ::bind(sockFd_, (struct sockaddr*) & serv, sizeof(serv));
	return ret;
}

int Socket::listen()
{
	int ret = ::listen(sockFd_, parameter::backLog);
	return ret;
}

ssize_t Socket::read(void* buf, size_t count)
{
	auto ret = ::read(sockFd_, buf, count);
	if (ret >= 0){
		return ret;
	}
	if(ret == -1 && errno == EINTR){
		return read(buf, count);
	}
	netco::Scheduler::getScheduler(0)->getProcessor(threadIdx)->waitEvent(sockFd_, EPOLLIN | EPOLLPRI | EPOLLRDHUP | EPOLLHUP);
	return ::read(sockFd_, buf, count);
}

void Socket::connect(const char* ip, int port){
	struct sockaddr_in addr = {0};
	addr.sin_family= AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr.sin_addr);
	ip_ = std::string(ip);
	port_ = port;
	auto ret = ::connect(sockFd_, (struct sockaddr*)&addr, sizeof(sockaddr_in));
	if(ret == 0){
		return;
	}
	if(ret == -1 && errno == EINTR){
		return connect(ip, port);
	}
	netco::Scheduler::getScheduler(0)->getProcessor(threadIdx)->waitEvent(sockFd_, EPOLLOUT);
	return connect(ip, port);
}

ssize_t Socket::send(const void* buf, size_t count)
{
	size_t sendIdx = ::send(sockFd_, buf, count, MSG_NOSIGNAL);
	if (sendIdx >= count){
		return count;
	}
	netco::Scheduler::getScheduler(0)->getProcessor(threadIdx)->waitEvent(sockFd_, EPOLLOUT);
	return send((char *)buf + sendIdx, count - sendIdx);
}


bool Socket::getSocketOpt(struct tcp_info* tcpi) const
{
	socklen_t len = sizeof(*tcpi);
	memset(tcpi, 0, sizeof(*tcpi));
	return ::getsockopt(sockFd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

bool Socket::getSocketOptString(char* buf, int len) const
{
	struct tcp_info tcpi;
	bool ok = getSocketOpt(&tcpi);
	if (ok)
	{
		snprintf(buf, len, "unrecovered=%u "
			"rto=%u ato=%u snd_mss=%u rcv_mss=%u "
			"lost=%u retrans=%u rtt=%u rttvar=%u "
			"sshthresh=%u cwnd=%u total_retrans=%u",
			tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
			tcpi.tcpi_rto,          // Retransmit timeout in usec
			tcpi.tcpi_ato,          // Predicted tick of soft clock in usec
			tcpi.tcpi_snd_mss,
			tcpi.tcpi_rcv_mss,
			tcpi.tcpi_lost,         // Lost packets
			tcpi.tcpi_retrans,      // Retransmitted packets out
			tcpi.tcpi_rtt,          // Smoothed round trip time in usec
			tcpi.tcpi_rttvar,       // Medium deviation
			tcpi.tcpi_snd_ssthresh,
			tcpi.tcpi_snd_cwnd,
			tcpi.tcpi_total_retrans);  // Total retransmits for entire connection
	}
	return ok;
}

std::string Socket::getSocketOptString() const
{
	char buf[1024];
	buf[0] = '\0';
	getSocketOptString(buf, sizeof buf);
	return std::string(buf);
}


int Socket::shutdownWrite()
{
	int ret = ::shutdown(sockFd_, SHUT_WR);
	return ret;
}

int Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(sockFd_, IPPROTO_TCP, TCP_NODELAY,
		&optval, static_cast<socklen_t>(sizeof optval));
	return ret;
}

int Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(sockFd_, SOL_SOCKET, SO_REUSEADDR,
		&optval, static_cast<socklen_t>(sizeof optval));
	return ret;
}

int Socket::setReusePort(bool on)
{
	int ret = -1;
#ifdef SO_REUSEPORT
	int optval = on ? 1 : 0;
	ret = ::setsockopt(sockFd_, SOL_SOCKET, SO_REUSEPORT,
		&optval, static_cast<socklen_t>(sizeof optval));
#endif
	return ret;
}

int Socket::setKeepAlive(bool on)
{
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(sockFd_, SOL_SOCKET, SO_KEEPALIVE,
		&optval, static_cast<socklen_t>(sizeof optval));
	return ret;
}

