#pragma once

#include "rpc_method.h"
#include "rpc_service.h"
#include "../../include/tcp/tcp_server.h"
#include "../spinlock_guard.h"
#include "name_service_register.h"
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
        RpcServerStub() : m_tcp_server(new TcpServer()), m_name_service_register(nullptr){}
        RpcServerStub(NameServiceRegister::Ptr name_service_register) 
            : m_tcp_server(new TcpServer()), m_name_service_register(name_service_register){}
        ~RpcServerStub(){
            UnregisterAll();
        }
        void SetNameServiceRegister(NameServiceRegister::Ptr name_service_register) { m_name_service_register = name_service_register; }
        void RegisterService(const std::string& service_name, const std::string& method_name, RpcMethod::method_callback_t callback);
        void process_request(const std::string& read_buffer, std::string& write_buffer);
        void start(const char* ip,int port);
        void start_multi(const char* ip,int port);
        void register_connection(std::function<void(netco::Socket*)>& conn);
        void registerAllService(const std::string& ipPortAddr);
        void UnregisterAll();
    private:
        std::map<std::string, RpcService::Ptr> m_service_map;
        Spinlock m_lock;
        std::unique_ptr<TcpServer> m_tcp_server;
        NameServiceRegister::Ptr m_name_service_register;
        std::string m_ipPortAddr;
    };
}