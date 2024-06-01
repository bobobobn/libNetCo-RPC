#include "../include/tcpserver.h"
#include "../include/socket.h"
#include <cassert>

using namespace netco;


/** 默认的server自定义函数*/
static conn_callback default_connection(
    [](netco::SocketPtr connect_socket)
    {
        /** 对这个改造的socket进行生命其管理*/
        
        /** 设置数据缓冲区*/
        std::vector<char> buf;
        buf.resize(2048);
        while(true)
        {
            // LOG_INFO("--------start one read-write process loop------------");
            /** readnum是实际上读到的数据*/
            auto readNum = connect_socket->read((void*)&(buf[0]), buf.size());	//这里也会发生协程切换(切回去是运行loop)
            //std::string ok = "HTTP/1.0 200 OK\r\nServer: minico/0.1.0\r\nContent-Type: text/html\r\n\r\n";
            if(readNum <= 0)
            {
                /** read = 0 exit*/
                break;
            }
            //connect_socket->send(ok.c_str(), ok.size());
            connect_socket->send((void*)&(buf[0]), readNum);
            /** 
             * 现在的情况是，rpc客户端对应的这个链接循环，做完所有的任务就挂掉了，需要退出
             * 因此下面的退出处理是必须的
             * */
            //if(readNum < (int)buf.size())
            //{
            //    break;
            //}
            // LOG_INFO("--------finish one read-write process loop------------");
        }
    }
);

void TcpServer::startServer(const char* ip, int port)
{
    if(serverConnectionCB_ == nullptr)
    {
        register_connection(default_connection);
    }
    listenFd_ = std::make_shared<Socket>();
    if (listenFd_->isUseful())
    {
        listenFd_->setTcpNoDelay(true);
        listenFd_->setReuseAddr(true);
        listenFd_->setReusePort(true);
        assert(listenFd_->bind(ip, port) >= 0);
        listenFd_->listen();
    }
    serverPort_ = port;
    if( ip == nullptr ){
        serverIP_ = "any ip";
    }
    else
    {
        serverIP_ = ip;
    }

    netco::co_go(std::bind(&TcpServer::serverLoop, this));

}

void TcpServer::serverLoop()
{
    if( nullptr == listenFd_ )
    {
        return;
    }
    while(1)
    {
        SocketPtr connFd = listenFd_->accept();
        connFd->setTcpNoDelay(true);
        netco::co_go( std::bind(*serverConnectionCB_, connFd) );
    }

}