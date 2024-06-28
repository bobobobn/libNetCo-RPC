#pragma once
#include "../../include/tcp/tcp_server.h"
#include "rpc_server_stub.h"
#include "../../include/json.h"

namespace netco{
    /**
     * @brief 基于协程的rpc服务器 秉持着最基础的复用tcpserver  在此之上封装一些rpc服务逻辑
    */
    class RpcServer
    {
    public:
        DISALLOW_COPY_MOVE_AND_ASSIGN(RpcServer);

        RpcServer() : m_rpc_server_stub(new RpcServerStub())
        {
            NETCO_LOG()<<("rpcserver constructor the rpc-server-stub");
            /** add a ping service*/
        }

        ~RpcServer(){
        NETCO_LOG()<<("rpcserver destructor the tcpserver");
        m_rpc_server_stub = nullptr;
        };

        /**
         * 开启rpc服务器的运行
         */
        void start(const char* ip,int port);
        
        void start_multi(const char* ip,int port);

        /** 
         * 向服务器中添加一种服务,一种服务可以有多个接口方法 
         */
        void add_service(const std::string& service_name, const std::string method_name, RpcChannel::method_callback_t callback)
        {
            m_rpc_server_stub->RegisterService(service_name, method_name, callback);
        }

    private:
        /** connection callback*/
        void on_connection(netco::Socket* conn);

        /** tcp server handle*/
        std::unique_ptr<RpcServerStub> m_rpc_server_stub;

        std::atomic<int> m_conn_number;


    };

}
