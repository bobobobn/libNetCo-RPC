#pragma once

#include "rpcheader.pb.h"
#include "rpc_response_header.pb.h"
#include "../../include/tcp/tcp_client.h"
#include "name_service.h"
#include "load_balancer.h"
#include "mutex_guard.h"

#include <memory>
#include <unordered_map>

namespace netco{
    /**
     * RPC客户端存根，负责网络消息的发送与编解码
    */
    class RpcClientStub
    {
    public:
        DISALLOW_COPY_MOVE_AND_ASSIGN(RpcClientStub);

        RpcClientStub() : m_name_service(nullptr)
        {
        }

        RpcClientStub(NameService::Ptr name_service) : m_name_service(name_service)
        {
        }
        
        ~RpcClientStub()
        {
        }
        
        void set_name_service(NameService::Ptr name_service)
        {
            m_name_service = name_service;
        }

        void call(const std::string& service_name, const std::string& method_name, const std::string& args, std::string& response, RpcResponseHeader& header);

    private:
        NameService::Ptr m_name_service;
        std::unordered_map<std::string, LoadBalancer::UniquePtr> m_load_balancer_map;
        MutexLock m_balancer_map_mutex;
        std::vector<char> buf;
    };
}
