#pragma once
#include <memory>
namespace netco{
    class NameServiceRegister{
    public:
        using Ptr = std::shared_ptr<NameServiceRegister>;
        virtual ~NameServiceRegister() {}
        virtual void RegisterMethod(const char* service_name, const char* method_name, const char* ipPort) = 0;
        virtual void UnregisterMethod(const char* service_name, const char* method_name, const char* ipPort) = 0;
    };
}