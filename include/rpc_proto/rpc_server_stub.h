#pragma once

#include "rpc_channel.h"
#include "rpc_service.h"
#include "../../include/tcp/tcp_server.h"
#include "../spinlock_guard.h"
#include "../../include/zk_client.h"
#include "../parameter.h"


/**
 * @brief The RpcHeader Struct
    message RpcHeader {
    string service_name = 1;
    string method_name = 2;
    uint32 args_size = 3;
}
 */
namespace netco{

    class RpcServerStub{
    public:
        RpcServerStub() : m_tcp_server(new TcpServer()), m_zk_client(new ZkClient(parameter::zkServerAddr)){}
        ~RpcServerStub(){}
        
        void RegisterService(const std::string& service_name, const std::string& method_name, RpcChannel::method_callback_t callback);
        void process_request(const std::string& read_buffer, std::string& write_buffer);
        void start(const char* ip,int port);
        void start_multi(const char* ip,int port);
        void register_connection(std::function<void(netco::Socket*)>& conn);
        void registerAllServiceOnZk(ZkClient::Ptr, const std::string& ipPortAddr);

    private:
        std::map<std::string, RpcSerivce::Ptr> m_service_map;
        Spinlock m_lock;
        std::unique_ptr<TcpServer> m_tcp_server;
        ZkClient::Ptr m_zk_client;
    };
}