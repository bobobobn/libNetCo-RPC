#include "../../include/rpc_proto/zk_naming_service.h"
#include "../log.h"
#include <set>
#include <algorithm>
namespace netco{
    static void child_watch_cb(zhandle_t *zh, int type, int state, const char *path, void *context){
        ZKNamingService* zk_ns = (ZKNamingService*)context;
        NETCO_LOG()<< "watcher_chld called with type " << type << " state " << state << " path " << path;
        NETCO_LOG()<< " ZOO_CHILD_EVENT:"<<ZOO_CHILD_EVENT<<" ZOO_SESSION_EVENT:"<<ZOO_SESSION_EVENT<<" ZOO_CREATED_EVENT:"<<ZOO_CREATED_EVENT<<" ZOO_DELETED_EVENT:"<<ZOO_DELETED_EVENT<<" ZOO_CHANGED_EVENT:"<<ZOO_CHANGED_EVENT<<" ZOO_CHILD_EVENT:"<<ZOO_CHILD_EVENT<<" ZOO_NOTWATCHING_EVENT:"<<ZOO_NOTWATCHING_EVENT;
        if(type == ZOO_CHILD_EVENT){
            int service_len;
            for(service_len = 1; service_len < strlen(path); service_len++){
                if(path[service_len] == '/'){
                    service_len--;
                    break;
                }
            }
            std::string service_name(path + 1, service_len);
            std::string method_name(path + service_len + 2);
            NETCO_LOG_FMT("Method %s of service %s is updated, updating servers", method_name.c_str(), service_name.c_str());
            if(zk_ns!= nullptr){
                zk_ns->RunNS(service_name.c_str(), method_name.c_str());
            }
            else{
                //"reset child watcher";
                zoo_wget_children(zh, path, child_watch_cb, context, nullptr);
            }
        }
    }
    void ZKNamingService::GetServers(const char* service_name, const char* method_name, std::vector<ServerNode>* servers){
        std::set<ServerNode> server_set;
        // get server list from zk
        struct String_vector children;
        char path[1024];
        snprintf(path, sizeof(path), "/%s/%s", service_name, method_name);
        _zk_client->list_children_and_watch(path, &children, child_watch_cb, this);
        for (int i = 0; i < children.count; ++i) {
            std::string child_name = children.data[i];
            std::string ipPort = child_name;
            std::string node_data = _zk_client->get((std::string(path) + "/" + child_name).c_str());
            // add server to set only if it is not already in the set
            if(server_set.emplace(ipPort).second){
                servers->emplace_back(ipPort);
            }
            NETCO_LOG_FMT("RpcClientStub::update_service_map: service_name: %s, method_name: %s, ipPort: %s", service_name, method_name, ipPort.c_str());
        }
        deallocate_String_vector(&children);
    }

    void ZKNamingService::RunNS(const char* service_name, const char* method_name){
        std::vector<ServerNode> presence_servers;
        GetServers(service_name, method_name, &presence_servers);
        char service_method_key[1024];
        snprintf(service_method_key, sizeof(service_method_key), "%s_%s", service_name, method_name);
        auto it = _actions.find(service_method_key);
        if(it == _actions.end()){
            NETCO_LOG_FMT("RpcClientStub::update_service_map: service_name: %s, method_name: %s, presence_servers: %s", service_name, method_name, presence_servers.empty() ? "empty" : "not empty");
            _actions[service_method_key] = std::make_shared<NameServiceActionImpl>();
            auto action = _actions[service_method_key];
            action->ResetServers(presence_servers);
        }
        else{
            NETCO_LOG_FMT("RpcClientStub::update_service_map: service_name: %s, method_name: %s, presence_servers: %s", service_name, method_name, presence_servers.empty() ? "empty" : "not empty");
            it->second->ResetServers(presence_servers);
        }
    }

    size_t ResetFn(std::vector<ServerNode>& last_servers, NameServiceActionImpl* action, std::vector<ServerNode>& new_servers){
        if( !action->_servers_to_add.empty() || !action->_servers_to_remove.empty() || action->_servers_list_changed){
            action->AddServers(last_servers);
            action->RemoveServers(last_servers);
            action->_servers_to_add.clear();
            action->_servers_to_remove.clear();
            action->_servers_list_changed = 0;
        }
        else{
            std::sort(last_servers.begin(), last_servers.end());
            std::sort(new_servers.begin(), new_servers.end());
            std::set_difference(last_servers.begin(), last_servers.end(), 
                                new_servers.begin(), new_servers.end(), 
                                std::back_inserter(action->_servers_to_remove));
            std::set_difference(new_servers.begin(), new_servers.end(),
                                last_servers.begin(), last_servers.end(),
                                std::back_inserter(action->_servers_to_add));
            if(!action->_servers_to_add.empty() || !action->_servers_to_remove.empty()){
                action->_servers_list_changed = 1;
                action->AddServers(last_servers);
                action->RemoveServers(last_servers);
            }
        }
        return 0;
    }

    void NameServiceActionImpl::ResetServers(std::vector<ServerNode>& new_servers){
        _servers_data.Modify(ResetFn, this, new_servers);
    }

    void NameServiceActionImpl::AddServers(std::vector<ServerNode>& last_servers){
        for(auto& server : _servers_to_add){
            last_servers.push_back(server);
        }
    }

    void NameServiceActionImpl::RemoveServers(std::vector<ServerNode>& last_servers){
        for(auto& server : _servers_to_remove){
            auto it = std::find(last_servers.begin(), last_servers.end(), server);
            if(it != last_servers.end()){
                std::swap(*it, last_servers.back());
                last_servers.pop_back();
            }
        }
    }

}