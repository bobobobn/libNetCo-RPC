#include "../../include/rpc_proto/rpc_client_stub.h"
#include "../../include/log.h"
#include "../../include/rpc_proto/rpcheader.pb.h"
#include "../../include/rpc_proto/rpc_response_header.pb.h"
#include "../../include/zk_client.h"
#include "../../include/rpc_proto/locality_aware_load_balancer.h"
#include "../../include/rpc_proto/short_socket_channel.h"
#include "../../include/utils.h"

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
        std::string service_method_key = service_name + "_" + method_name;
        // get load balancer to select server and get a socket channel connected to the selected server
        {
            MutexGuard lock(m_balancer_map_mutex);
            auto it = m_load_balancer_map.find(service_name);
            if (it == m_load_balancer_map.end()){
                m_load_balancer_map[service_name] = LocalityAwareLoadBalancer::New(ShortSocketChannel::New);
                m_name_service->ManageLoadBalancer(service_name.c_str(), method_name.c_str(), &m_load_balancer_map[service_name]);
            }
        }
        LoadBalancer::SelectIn in{utils::gettimeofday_us(), false, false, 0};
        LoadBalancer::SelectOut out;
        int ret = m_load_balancer_map[service_name]->select_server(in, &out);
        // send request to server and get response
        if (ret == 0){
            auto& socket_channel = out.ptr;
            // request: --{header_len}{rpc_header}{args}--
            RpcHeader rpc_header;
            rpc_header.set_service_name(service_name);
            rpc_header.set_method_name(method_name);
            rpc_header.set_args_size(args.size());

            std::string rpc_request_str;
            rpc_header.SerializeToString(&rpc_request_str);
            uint32_t rpc_header_len = rpc_request_str.size();
            rpc_request_str.insert(0, (char*)&rpc_header_len, sizeof(rpc_header_len));
            rpc_request_str.append(args);
            socket_channel->send(rpc_request_str.c_str(), rpc_request_str.size());
            // response: --{header_len}{response_header}{args}--
            std::vector<char> rpc_response(4096);
            int recv_len = socket_channel->recv(rpc_response.data(), rpc_response.size());
            // feedback to load balancer
            if( out.need_feedback ){
                m_load_balancer_map[service_name]->Feedback({in.begin_time_us, service_method_key, rpc_response_header.status()});
            }
            rpc_header_len = *(uint32_t*)rpc_response.data();
            rpc_response_header.ParseFromArray(rpc_response.data() + sizeof(rpc_header_len), rpc_header_len);
            response.assign(rpc_response.data() + sizeof(rpc_header_len) + rpc_header_len, rpc_response_header.args_size());
            NETCO_LOG()<<"response code:"<<rpc_response_header.status()
            <<"rpc response message:"<<rpc_response_header.message()
            <<"rpc response args size:"<<rpc_response_header.args_size();

        }
    }
}

