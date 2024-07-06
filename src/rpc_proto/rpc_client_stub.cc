#include "../../include/rpc_proto/rpc_client_stub.h"
#include "../../include/log.h"
#include "../../include/rpc_proto/rpcheader.pb.h"
#include "../../include/rpc_proto/rpc_response_header.pb.h"
#include "../../include/zk_client.h"

/**
 * @brief rpc response code
 *  SUCCESS = 0;
 *  UNKNOWN_ERROR = 1;
 *  INVALID_REQUEST = 2;
 *  AUTH_FAILED = 3;
 *  PERMISSION_DENIED = 4;
 */

namespace netco{
    void RpcClientStub::call(const std::string& service_name, const std::string& method_name, const std::string& args, std::string& response, RpcResponseHeader& rpc_response_header){
        if(m_service_method_map[service_name].empty()){
            update_service_map(service_name);
        }
        std::string server_addr;
        NETCO_LOG_FMT("RpcClientStub::call: service_name: %s, method_name: %s has %d servers", service_name.c_str(), method_name.c_str(), m_service_method_map[service_name][method_name].size()); 
        for(auto& ipPort : m_service_method_map[service_name][method_name]){
            server_addr = ipPort;
            break;
        }
        if (server_addr.empty()){
            NETCO_LOG_ERROR(NETCO_LOG_ROOT())<<"RpcClientStub::call: server_addr is empty";
            return;
        }
        size_t pos = server_addr.find(":");
        if (pos == std::string::npos){
            NETCO_LOG_ERROR(NETCO_LOG_ROOT())<<"RpcClientStub::call: server_addr format error";
            return;
        }
        std::string ip = server_addr.substr(0, pos);
        int port = std::stoi(server_addr.substr(pos + 1));
        NETCO_LOG_FMT("RpcClientStub::call: service_name: %s, method_name: %s found on zk, server_addr: %s:%d", service_name.c_str(), method_name.c_str(), ip.c_str(), port);
        connect(ip.c_str(), port);

        NETCO_LOG_FMT("RpcClientStub::call: service_name: %s, method_name: %s", service_name.c_str(), method_name.c_str());      
        RpcHeader rpc_header;
        rpc_header.set_service_name(service_name);
        rpc_header.set_method_name(method_name);
        rpc_header.set_args_size(args.size());


        std::string rpc_request_str;
        rpc_header.SerializeToString(&rpc_request_str);
        uint32_t rpc_header_len = rpc_request_str.size();
        rpc_request_str.insert(0, (char*)&rpc_header_len, sizeof(rpc_header_len));
        rpc_request_str.append(args);
        m_tcp_client->send(rpc_request_str.c_str(), rpc_request_str.size());

        std::vector<char> rpc_response(4096);
        int recv_len = m_tcp_client->recv(rpc_response.data(), rpc_response.size());
        rpc_header_len = *(uint32_t*)rpc_response.data();
        rpc_response_header.ParseFromArray(rpc_response.data() + sizeof(rpc_header_len), rpc_header_len);
        response.assign(rpc_response.data() + sizeof(rpc_header_len) + rpc_header_len, rpc_response_header.args_size());
        NETCO_LOG()<<"response code:"<<rpc_response_header.status()
        <<"rpc response message:"<<rpc_response_header.message()
        <<"rpc response args size:"<<rpc_response_header.args_size();

    }

    void RpcClientStub::update_service_map(const std::string& service_name){
        struct String_vector children;
        const std::string path = "/" + service_name + "/provider";
        m_zk_client->list_children(path.c_str(), &children, this);
        m_service_method_map[service_name].clear();
        for (int i = 0; i < children.count; ++i) {
            std::string child_name = children.data[i];
            std::string method_name = child_name.substr(0, child_name.find("_"));
            std::string ipPort = m_zk_client->get((path+ "/" + child_name).c_str());
            m_service_method_map[service_name][method_name].insert(ipPort);
            NETCO_LOG_FMT("RpcClientStub::update_service_map: service_name: %s, method_name: %s, ipPort: %s", service_name.c_str(), method_name.c_str(), ipPort.c_str());
        }
        deallocate_String_vector(&children);
    }

    void RpcClientStub::watch_service(const std::string& service_name){
        m_zk_client->create( ("/" + service_name + "/consumer/watcher").c_str(), nullptr ,ZOO_EPHEMERAL_SEQUENTIAL);
        
    }

}