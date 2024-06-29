#pragma once
#include <zookeeper/zookeeper.h>
#include <string>
#include <memory>

namespace netco{
    class ZkClient {
    public:
        using Ptr = std::shared_ptr<ZkClient>;
        ZkClient(const char* hosts);
        ~ZkClient();
        int connect();
        int close();
        int create(const char* path, const char* data, int flags, struct ACL_vector* acl = &ZOO_OPEN_ACL_UNSAFE);
        int delete_(const char* path, int version);
        int exists(const char* path, int watch, struct Stat* stat);
        std::string get(const char* path, int watch = 0, struct Stat* stat = nullptr);
        int set(const char* path, const char* value, int version);
        int getChildren(const char* path, int watch, struct String_vector* children);
        int getAcl(const char* path, struct ACL_vector** acl, struct Stat* stat);
        int setAcl(const char* path, int version, const struct ACL_vector* acl);
        int addAuth(const char* scheme, const char* auth);
    private:
        zhandle_t* zh_;
    };
}
