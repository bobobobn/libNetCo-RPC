#pragma once

#include "../../include/rpc_proto/rpc_server_stub.h"
#include "../../include/rpc_proto/rpc_response_header.pb.h"
#include <string>
#include <functional>
#include <map>
#include <cstring>
/**
 * @brief The RpcHeader Struct
    message RpcHeader {
    string service_name = 1;
    string method_name = 2;
    uint32 args_size = 3;
}
 */
namespace netco{
    void RpcServerStub::start(const char* ip,int port)
    {
        /** 开启tcp服务器运行*/
        m_tcp_server->start(ip,port); 
        NETCO_LOG()<<("rpc-server-stub start run the tcp-server loop");
    }

    void RpcServerStub::start_multi(const char* ip,int port)
    {
        m_tcp_server->start_multi(ip,port);
        NETCO_LOG()<<("rpc-server-stub start run the tcp-server multi loop");
    }

    void RpcServerStub::register_connection(std::function<void(netco::Socket*)>& conn)
    {
        m_tcp_server->register_connection(conn);
    }
    void RpcServerStub::RegisterService(const std::string& service_name, const std::string& method_name, RpcChannel::method_callback_t callback){
        SpinlockGuard guard(m_lock);
        if(m_service_map.find(service_name) == m_service_map.end()){
            // register new service
            NETCO_LOG()<<"register new service:"<<service_name;
            m_service_map[service_name] = std::make_shared<RpcSerivce>(service_name);
            m_service_map[service_name]->register_method(method_name, callback);
        }
        else{
            m_service_map[service_name]->register_method(method_name, callback);
        }
    }

    void RpcServerStub::process_request(const std::string& read_buffer, std::string& write_buffer){
        uint32_t head_size = 0;
        static_cast<void>( memcpy(&head_size, read_buffer.data(), sizeof(head_size)) ); // read head size
        ntohl(head_size);
        NETCO_LOG()<<("rpc-server-stub process request head size: " + std::to_string(head_size));
        RpcHeader header;
        header.ParseFromString(read_buffer.substr(sizeof(head_size), head_size)); // read header

        std::string service_name = header.service_name();
        std::string method_name = header.method_name();
        uint32_t args_size = header.args_size();
        NETCO_LOG()<<("rpc-server-stub process request service_name: " + service_name + " method_name: " + method_name + " args_size: " + std::to_string(args_size))<<" arg_size_in_buffer: "<<(read_buffer.size()-(sizeof(head_size) + head_size));
        RpcResponseHeader response_header;
        std::string response_header_str;
        std::string response;
        if(m_service_map.find(service_name) == m_service_map.end()){
            // handle service not found error
            response_header.set_status(INVALID_REQUEST);
            response_header.set_message("service not found");
            response_header.set_args_size(0);
        }        
        else{
            // handle request
            RpcSerivce::Ptr service = m_service_map[service_name];
            if(service->has_method(method_name)){
                NETCO_LOG()<<"calling method:"<<method_name;
                response = service->call_method(method_name, read_buffer.substr(sizeof(head_size) + head_size, args_size));
                response_header.set_status(SUCCESS);
                response_header.set_message("success");
                response_header.set_args_size(response.size());
            }                
            else{
                // handle method not found error
                response_header.set_status(INVALID_REQUEST);
                response_header.set_message("method not found");
                response_header.set_args_size(0);
            }
        }
        // write response
        write_buffer.clear();
        response_header.SerializeToString(&response_header_str); // write response header
        uint32_t response_header_size = response_header_str.size();
        write_buffer.append(reinterpret_cast<const char*>(&response_header_size), sizeof(response_header_size)); // write response header size
        write_buffer.append(response_header_str); // write response header
        write_buffer.append(response); // write responses
    }

}