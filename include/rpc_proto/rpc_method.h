#pragma once

#include "rpcheader.pb.h"
#include <string>
#include <functional>
#include <memory>

namespace netco{
    class RpcMethod{
    public:
        using Ptr = std::shared_ptr<RpcMethod>;
        using method_callback_t = std::function<std::string(const std::string&)>;
        RpcMethod(std::string method_name, method_callback_t method_callback):
            m_method_name(method_name), m_method_callback(method_callback){}
        ~RpcMethod(){}
        std::string callMethod(const std::string& arg){
            return m_method_callback(arg);
        }
        std::string get_method_name() const{
            return m_method_name;
        }
    private:
        std::string m_method_name;
        method_callback_t m_method_callback;
    };
}
