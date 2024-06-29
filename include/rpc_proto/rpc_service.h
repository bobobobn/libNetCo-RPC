#pragma once

#include "rpc_channel.h"
#include <string>
#include <functional>
#include <map>
#include "../log.h"
#include "../zk_client.h"

namespace netco{

    class RpcSerivce {
    public:
        using Ptr = std::shared_ptr<RpcSerivce>;
        RpcSerivce(std::string service_name) : m_service_name(service_name) {}
        ~RpcSerivce() {}

        bool register_method(std::string method_name, typename netco::RpcChannel::method_callback_t method_callback) {
            NETCO_LOG()<<"register method "<<method_name<<" for service "<<m_service_name;
            typename netco::RpcChannel::Ptr channel = std::make_shared<netco::RpcChannel>(method_name, method_callback);
            return register_method(channel);
        }
        
        bool register_method(typename netco::RpcChannel::Ptr channel) {
            std::string method_name = channel->get_method_name();
            if(channel->get_method_name().empty())
                return false;
            m_channels[method_name] = channel;
            return true;
        }
        bool has_method(std::string method_name) { return m_channels.find(method_name)!= m_channels.end(); }        
        std::string call_method(std::string method_name, const std::string& arg) {
            auto channel = m_channels.find(method_name);
            if (channel == m_channels.end()) {
                return nullptr;
            }
            NETCO_LOG()<<"find method "<<method_name<<" for service "<<m_service_name<<"calling...";
            return channel->second->callMethod(arg);
        }
        void registerAllMethodOnZk(ZkClient::Ptr zkClient, const std::string& ipPortAddr){
            zkClient->create( ("/" + m_service_name).c_str(), nullptr, ZOO_PERSISTENT);
            for(auto& channel : m_channels){
                NETCO_LOG()<<"register method "<<channel.first<<" for service "<<m_service_name<<" on zk..."<<" method running on: "<< ipPortAddr;
                zkClient->create(("/" + m_service_name + "/" + channel.first).c_str(), ipPortAddr.c_str(), ZOO_EPHEMERAL);
            }
        }

    private:
        std::string m_service_name;
        std::map<std::string, netco::RpcChannel::Ptr> m_channels;
    };
}