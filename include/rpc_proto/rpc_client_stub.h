#pragma once

#include "rpcheader.pb.h"
#include "rpc_response_header.pb.h"
#include "../../include/tcp/tcp_client.h"
#include "../zk_client.h"

#include <memory>

namespace netco{
    /**
     * RPC客户端存根，负责网络消息的发送与编解码
    */
    class RpcClientStub
    {
    public:
        DISALLOW_COPY_MOVE_AND_ASSIGN(RpcClientStub);

        RpcClientStub() : m_tcp_client(new TcpClient()), m_zk_client(new ZkClient(parameter::zkServerAddr))
        {
            NETCO_LOG()<<("rpc client constructor a tcp client");
        }
        
        ~RpcClientStub()
        {
            m_tcp_client = nullptr;
            NETCO_LOG()<<("rpc_client destructor a tcp client");
        }
        
        void connect(const char* ip,int port)
        {
            return m_tcp_client->connect(ip,port);
        }

        int close()
        {
            return m_tcp_client->disconnect();
        }
        void call(const std::string& service_name, const std::string& method_name, const std::string& args, std::string& response, RpcResponseHeader& header);
        void update_service_map(const std::string& service_name);
    private:
        void watch_service(const std::string& service_name);
    private:
        std::unique_ptr<TcpClient> m_tcp_client;
        ZkClient::Ptr m_zk_client;
        std::vector<char> buf;
        // service_name -> method_name -> set<ipPort>
        std::map<std::string, std::map<std::string, std::set<std::string> > > m_service_method_map;
    };
}
