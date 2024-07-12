#pragma once
#include "name_service_register.h"
#include "../zk_client.h"
#include "../parameter.h"
#include "../log.h"
#include <string>
namespace netco{
    /**
     * @brief A name service register implementation based on Zookeeper.
     * Structure of the znode:
     * /service_name/method_name/ipPort1
     * /service_name/method_name/ipPort2
     *...
     */
    class ZkNameServiceRegister : public NameServiceRegister{
    public:
        using Ptr = std::shared_ptr<ZkNameServiceRegister>;
        ZkNameServiceRegister() : zk_client_(new ZkClient(parameter::zkServerAddr)) {}
        void RegisterMethod(const char* service_name, const char* method_name, const char* ipPort) override;
        void UnregisterMethod(const char* service_name, const char* method_name, const char* ipPort) override;
        static ZkNameServiceRegister::Ptr New() { return std::make_shared<ZkNameServiceRegister>(); }
    private:
        ZkClient::Ptr zk_client_;
    };
    void ZkNameServiceRegister::RegisterMethod(const char* service_name, const char* method_name, const char* ipPort){
        std::string service_name_str(service_name);
        std::string method_name_str(method_name);
        std::string path;

        // the service path
        path = "/" + service_name_str;
        zk_client_->create( path.c_str(), nullptr, ZOO_PERSISTENT);

        // the parent path of all the same method
        path += "/" + method_name_str;
        zk_client_->create( path.c_str(), nullptr, ZOO_PERSISTENT);

        // the ipPorts
        path += "/" + std::string(ipPort);
        zk_client_->create(path.c_str(), /*add server data here*/nullptr, ZOO_EPHEMERAL);
    }
    void ZkNameServiceRegister::UnregisterMethod(const char* service_name, const char* method_name, const char* ipPort){
        std::string path = "/";
        path += service_name;
        path += "/";
        path += method_name;
        path += "/";
        path += ipPort;
        if(zk_client_->delete_node_no_version_check(path.c_str())){
            NETCO_LOG_FMT("Unregister method %s of service %s failed, ipPort %s", method_name, service_name, ipPort);
        }
        else{
            NETCO_LOG_FMT("Unregister method %s of service %s success, ipPort %s", method_name, service_name, ipPort);
        }
    }
}