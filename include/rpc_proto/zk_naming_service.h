#pragma once
#include <map>
#include "name_service.h"
#include "../zk_client.h"
#include "../parameter.h"
#include "../container/doubly_buffered_data.h"
#include "locality_aware_load_balancer.h"

namespace netco{
    
    class NameServiceActionImpl : public NameServiceAction {
    public:
        using UniquePtr = std::unique_ptr<NameServiceActionImpl>;
        using Ptr = std::shared_ptr<NameServiceActionImpl>;
        NameServiceActionImpl() : _servers_list_changed(0), _load_balancer(nullptr) {}
        NameServiceActionImpl(LoadBalancer::UniquePtr* load_balancer) : _servers_list_changed(0), _load_balancer(load_balancer) {}
        void ResetServers(std::vector<ServerNode>& servers) override;
        NameService::DataPtr GetServerListPtr() override
        {
            NameService::DataPtr ptr(new NameService::DataType());
            _servers_data.Read(*ptr);
            return ptr;
        }
        void SetLoadBalancer(LoadBalancer::UniquePtr* load_balancer) { _load_balancer = load_balancer; }
        void LoadBalancerReset() {
            if(_load_balancer == nullptr) return;
            for(const auto& server : _servers_to_add) {
                (*_load_balancer)->add_server(server.ipPort_);
            }
            for(const auto& server : _servers_to_remove) {
                (*_load_balancer)->remove_server(server.ipPort_);
            }
        }
    private:
        friend size_t ResetFn(std::vector<ServerNode>& last_servers, NameServiceActionImpl*, std::vector<ServerNode>& new_servers);
        void AddServers(std::vector<ServerNode>& last_servers);
        void RemoveServers(std::vector<ServerNode>& last_servers);
    private:
        DoublyBufferedData<std::vector<ServerNode>> _servers_data;
        std::vector<ServerNode> _servers_to_remove;
        std::vector<ServerNode> _servers_to_add;
        int _servers_list_changed;
        LoadBalancer::UniquePtr* _load_balancer;
    };
    
    class ZKNamingService : public NameService {
    public:
        using Ptr = std::shared_ptr<ZKNamingService>;
        ZKNamingService() : _zk_client(std::make_shared<ZkClient>(parameter::zkServerAddr)) {}
        ~ZKNamingService(){};
        
        void ManageLoadBalancer(const char* service_name, const char* method_name, LoadBalancer::UniquePtr* load_balancer) override{
            NameServiceActionImpl::UniquePtr action(new NameServiceActionImpl());
            action->SetLoadBalancer(load_balancer);
            _actions[std::string(service_name) + "_" + std::string(method_name)] = std::move(action);
            RunNS(service_name, method_name);
        }
        // update the server list of the given service and method
        void RunNS(const char* service_name, const char* method_name) override;  
        /**
         * @brief Get a unique_ptr of the ScopePtr for the server list of the given service and method.
         *        You should release this pointer when you don't need it anymore.
         * @param service_name The name of the service.
         * @param method_name The name of the method.
         * @return A unique_ptr of the ScopePtr for the server list of the given service and method.
         */
        NameService::DataPtr GetServerListPtr(const char* service_name, const char* method_name) override{
            auto it = _actions.find(std::string(service_name) + "_" + std::string(method_name));
            if(it == _actions.end()){
                RunNS(service_name, method_name);
                return _actions[std::string(service_name) + "_" + std::string(method_name)]->GetServerListPtr();
            }            
            return it->second->GetServerListPtr();
        }
        static Ptr New() { return std::make_shared<ZKNamingService>(); }
    private:      
        void GetServers(const char* service_name, const char* method_name, std::vector<ServerNode>* servers) override;
        ZkClient::Ptr _zk_client;
        std::map<std::string, NameServiceAction::UniquePtr> _actions;
    };

}