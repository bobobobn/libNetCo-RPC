#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../container/doubly_buffered_data.h"

namespace netco{
    struct ServerNode {
        std::string ipPort_;
        int weight_;
        ServerNode(const std::string& ipPort) : ipPort_(ipPort), weight_(1) {}
        ServerNode(const std::string& ipPort, int weight) : ipPort_(ipPort), weight_(weight) {}
        bool operator==(const ServerNode& other) const {
            return ipPort_ == other.ipPort_;
        }

        bool operator!=(const ServerNode& other) const {
            return ipPort_ != other.ipPort_;
        }
        bool operator<(const ServerNode& other) const {
            return ipPort_ < other.ipPort_;
        }

        bool operator>(const ServerNode& other) const {
            return ipPort_ > other.ipPort_;
        }
    };

    class NameService{
        public:        
            using Ptr = std::shared_ptr<NameService>;
            using DataType = DoublyBufferedData<std::vector<ServerNode>>::ScopePtr;
            using DataPtr = std::unique_ptr<DataType>;
            virtual ~NameService(){};
            virtual void RunNS(const char* service_name, const char* method_name) = 0;
            virtual void GetServers(const char* service_name, const char* method_name, std::vector<ServerNode>*) = 0;
            virtual NameService::DataPtr GetServerListPtr(const char* service_name, const char* method_name) = 0;
    };
    class NameServiceAction{
    public:
        using Ptr = std::shared_ptr<NameServiceAction>;
        virtual ~NameServiceAction(){};
        virtual void ResetServers(std::vector<ServerNode>& servers) = 0;
        virtual NameService::DataPtr GetServerListPtr() = 0;
    };
}