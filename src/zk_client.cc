#pragma once
#include "../include/zk_client.h"
#include "../include/log.h"
#include <semaphore.h>

namespace netco{
    static void watcher(zhandle_t *zh, int type, int state, const char *path, void *context) {
        if(state == ZOO_CONNECTED_STATE){
            sem_t* sem = (sem_t*)context;
            sem_post(sem);
            NETCO_LOG_FMT("Watcher callback called with type %d, state %d, path %s\n", type, state, path);
        }
    }
    ZkClient::  ZkClient(const char* host) : zh_(nullptr){
        sem_t sem;
        sem_init(&sem, 0, 0);
        NETCO_LOG()<<"Creating ZkClient"<<" host: "<<host;
        zh_ = zookeeper_init(host, watcher, 10000, 0, (void*)&sem, 0);
        sem_wait(&sem);
        sem_destroy(&sem);
        NETCO_LOG()<<"ZkClient created successfully";
    }           
    ZkClient::~ZkClient(){
        if(zh_!= nullptr){
            zookeeper_close(zh_);
            NETCO_LOG()<<"ZkClient destroyed successfully";
        }
    }
    /**
     * @brief create a node with the given path, data, flags and ACLs
     * @param path the path of the node to be created
     * @param data the data to be stored in the node
     * @param flags the flags to be used for creating the node
     * @param acl the ACLs to be used for creating the node
     * @return 0 if the node is created successfully, -1 otherwise
     * @param flags can be one of the following:
     * ZOO_PERSISTENT：持久节点
    *  ZOO_EPHEMERAL：临时节点
    *  ZOO_PERSISTENT_SEQUENTIAL:持久顺序节点
    *  ZOO_EPHEMERAL_SEQUENTIAL:临时顺序节点
    *  ZOO_CONTAINER:容器节点
    * ZOO_PERSISTENT_WITH_TTL:带有TTL的持久节点
    * ZOO_PERSISTENT_SEQUENTIAL_WITH_TTL:带有TTL的持久顺序节点
    * 
    */
    int ZkClient::create(const char* path, const char* data, int flags, struct ACL_vector* acl){
        NETCO_LOG()<<"Creating node with path: "<<path<<" data: "<<data<<" flags: "<<flags<<" acl: "<<acl;
        if(path[0] != '/'){
            NETCO_LOG_FMT("Invalid path %s", path);
            return -1;
        }
        int data_len = 0;
        if(nullptr != data){
            data_len = strlen(data);
        }
        if(zoo_exists(zh_, path, 0, nullptr) == ZNONODE){
            if(zoo_create(zh_, path, data, data_len, acl, flags, nullptr, 0) == ZOK)
            {
                NETCO_LOG_FMT("Node %s created successfully", path);
                return 0;
            }
            else
            {
                NETCO_LOG_FMT("Failed to create node %s", path);
                return -1;
            }
        }
        else
        {
            NETCO_LOG_FMT("Node %s already exists", path);
            return -1;
        }
    }
    std::string ZkClient::get(const char* path, int watch, struct Stat* stat){
        char buffer[64];
        int buffer_len = sizeof(buffer);
        int rc = zoo_get(zh_, path, watch, buffer, &buffer_len, stat);
        NETCO_LOG()<<"data: "<<buffer;
        if(rc == ZOK){
            NETCO_LOG_FMT("Node %s value retrieved successfully", path);
            return std::string(buffer);
        }
        else{
            NETCO_LOG_FMT("Failed to retrieve node %s value", path);
            return nullptr;
        }
    }       
    int ZkClient::set(const char* path, const char* value, int version){
        int rc = zoo_set(zh_, path, value, strlen(value), version);
        if(rc == ZOK){
            NETCO_LOG_FMT("Node %s value set successfully", path);
            return 0;
        }
        else{
            NETCO_LOG_FMT("Failed to set node %s value", path);
            return -1;
        }
    }
    
}