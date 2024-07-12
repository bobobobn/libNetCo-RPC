#pragma once
#include <zookeeper/zookeeper.h>
#include <string>
#include <memory>

namespace netco{
    class ZkClient {
    public:
        typedef void (*watcher_fn)(zhandle_t *zh, int type,
            int state, const char *path,void *watcherCtx);
        using Ptr = std::shared_ptr<ZkClient>;
        ZkClient(const char* hosts);
        ~ZkClient();
        int create(const char* path, const char* data, int flags, struct ACL_vector* acl = &ZOO_OPEN_ACL_UNSAFE);
        std::string get(const char* path, int watch = 0, struct Stat* stat = nullptr);
        int set(const char* path, const char* value, int version);
        void list_children(const char *path, String_vector*, void *);
        std::string get_watch(const char* path, struct Stat* stat, void* stub);
        int delete_node_no_version_check(const char* path);
        void list_children_and_watch (const char* path, struct String_vector* children, watcher_fn child_watch_cb,void* client_stub);
    private:
        zhandle_t* zh_;
    };
}
