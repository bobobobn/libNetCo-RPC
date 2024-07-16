#pragma once

#include <functional>
#include <memory>
#include <sys/sysinfo.h>

#include "../../include/log.h"
#include "../../include/socket.h"
#include "../../include/netco_api.h"

/**
 * @brief: 客户端 一个客户端只用来保存一个连接 客户端必须在一个协程中运行
 */
class TcpClient
{
public:
    /** 进行socket的一系列初始化的工作*/
    TcpClient() : m_client_socket(new netco::Socket())
    {
        NETCO_LOG_FMT("%s", "tcpclient constructor a connection socket");
    }

    DISALLOW_COPY_MOVE_AND_ASSIGN(TcpClient);

    virtual ~TcpClient()
    {
        delete m_client_socket;
        m_client_socket = nullptr;
        NETCO_LOG_FMT("%s", "tcpclient destructor itself and the connection socket");
    }
    
    int connect(const char* ip,int port);
    void reset_socket();
    /** return 0 is success -1 is error*/
    int disconnect();
    size_t recv(void* buf,size_t count);
    size_t send(const void* buf,size_t count);
    
    inline int socket() const { return m_client_socket->fd();}
    
private:    
    netco::Socket* m_client_socket;
};


















