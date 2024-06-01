#pragma once
#include "netco_api.h"
#include "socket.h"
#include <memory>
#include <functional>


namespace netco
{
    using conn_callback = std::function<void(netco::SocketPtr)>;
    class TcpServer
    {
    public:
        TcpServer()
            : serverPort_(-1), serverIP_(nullptr), listenFd_(nullptr), serverConnectionCB_(nullptr)
        {}
        virtual ~TcpServer()
        {
            serverIP_ = nullptr;
        }
        void startServer(const char* ip, int port);
        inline void register_connection(const conn_callback& func)
        {
            this->register_connection_impl(func);
        }
        inline void register_connection(const conn_callback&& func)
        {
            this->register_connection_impl(std::move(func));
        }
    private:
        template<typename Func>
        inline void register_connection_impl(Func&& func) {
        serverConnectionCB_ = std::make_shared<conn_callback>(std::forward<Func>(func));
    }
        void serverLoop();        

    private:
        int serverPort_;
        const char* serverIP_;
        netco::SocketPtr listenFd_;
        std::shared_ptr<conn_callback> serverConnectionCB_;


    };
}