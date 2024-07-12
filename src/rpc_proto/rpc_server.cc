#include "../../include/rpc_proto/rpc_server_stub.h"
#include "../../include/rpc_proto/rpc_server.h"
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
    void RpcServer::start(const char* ip,int port)
    {
        std::function<void(netco::Socket*)> on_connection = 
            std::bind(&RpcServer::on_connection,this,std::placeholders::_1);
        
        /** register the connection callback*/
        m_rpc_server_stub->register_connection(on_connection);
        NETCO_LOG()<<("register the rpc-server-stub connection callback");
        /** register the service on zookeeper */

        m_rpc_server_stub->start(ip,port);
    }

    void RpcServer::start_multi(const char* ip,int port)
    {
        std::function<void(netco::Socket*)> on_connection = 
            std::bind(&RpcServer::on_connection,this,std::placeholders::_1);
        
        /** register the connection callback*/
        m_rpc_server_stub->register_connection(on_connection);
        NETCO_LOG()<<("register the rpc-server-stub connection callback");

        m_rpc_server_stub->start_multi(ip,port);
    }

    void RpcServer::on_connection(netco::Socket* conn)
    {
        /** 进行conn-fd的生命期管理*/
        std::shared_ptr<netco::Socket> connection(conn);

        /** add one client connection*/

        RpcHeader rpc_header;
        std::vector<char> buf(4096);
        std::string write_buf;

        int rpc_recv_message_len = 0;
        /** 
         * 收到了客户端发出的rpc请求,会做出如下处理 先不考虑错误处理
         * rpc请求会先收到一个头部信息,用于后续的主体信息流的截取
         * 两次接收 一次发送 
         */
        while(true)
        {
            int rpc_message_len = connection->read((void*)buf.data(), buf.size());
            if(rpc_message_len == 0)
            {
                NETCO_LOG()<<"client disconnect";
                break;
            }
            std::string read_buf(buf.data(), rpc_message_len);
            m_rpc_server_stub->process_request(read_buf, write_buf);
            connection->send(write_buf.c_str(), write_buf.size());
        }
    }
}