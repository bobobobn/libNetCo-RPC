#pragma once

#include "rpc_method.h"
#include <string>
#include <functional>
#include <map>
#include "../log.h"
#include "name_service_register.h"

namespace netco{

    class RpcService {
    public:
        using Ptr = std::shared_ptr<RpcService>;
        RpcService(std::string service_name) : m_service_name(service_name) {}
        ~RpcService() {}

        bool register_method(std::string method_name, typename netco::RpcMethod::method_callback_t method_callback) {
            NETCO_LOG()<<"register method "<<method_name<<" for service "<<m_service_name;
            typename netco::RpcMethod::Ptr method = std::make_shared<netco::RpcMethod>(method_name, method_callback);
            return register_method(method);
        }
        
        bool register_method(typename netco::RpcMethod::Ptr method) {
            std::string method_name = method->get_method_name();
            if(method->get_method_name().empty())
                return false;
            m_methods[method_name] = method;
            return true;
        }
        bool has_method(std::string method_name) { return m_methods.find(method_name)!= m_methods.end(); }        
        std::string call_method(std::string method_name, const std::string& arg) {
            auto method = m_methods.find(method_name);
            if (method == m_methods.end()) {
                return nullptr;
            }
            NETCO_LOG()<<"find method "<<method_name<<" for service "<<m_service_name<<"calling...";
            return method->second->callMethod(arg);
        }
        void registerAllMethod(NameServiceRegister::Ptr ns_register, const std::string& ipPortAddr){
            if(ns_register == nullptr){
                NETCO_LOG()<<"ns_register is nullptr";
                exit(1);
            }
            for(auto& method : m_methods){
                ns_register->RegisterMethod(m_service_name.c_str(), method.first.c_str(), ipPortAddr.c_str());
            }
        }

    private:
        std::string m_service_name;
        std::map<std::string, netco::RpcMethod::Ptr> m_methods;
    };
}