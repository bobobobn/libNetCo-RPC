#include "../../include/rpc_proto/rpc_client.h"
#include "../../include/rpc_proto/rpcheader.pb.h"

namespace netco{
    void RpcClient::call(const std::string& service_name, const std::string& method_name, const std::string& args, std::string& response, RpcResponseHeader& header){
        m_rpc_client_stub->call(service_name, method_name, args, response, header);
    }
}
