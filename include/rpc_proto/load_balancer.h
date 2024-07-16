#pragma once
#include <memory>
#include <vector>
#include "socket_channel.h"
namespace netco{
    class LoadBalancer{
    public:
        using ServerName = std::string;
        using Ptr = std::shared_ptr<LoadBalancer>;
        using UniquePtr = std::unique_ptr<LoadBalancer>;
        using NewFn = std::function<UniquePtr()>;
        struct SelectIn {
            int64_t begin_time_us;
            // Weight of different nodes could be changed.
            bool changable_weights;
            bool has_request_code;
            uint64_t request_code;
        };

        struct SelectOut {
            SelectOut()
                : ptr(nullptr), need_feedback(false) {}
            SocketChannel::UniquePtr ptr;
            ServerName node;
            bool need_feedback;
        };

        struct CallInfo {
            // Exactly same with SelectIn.begin_time_us, may be different from
            // controller->_begin_time_us which is beginning of the RPC.
            int64_t begin_time_us;
            // Remote side of the call.
            ServerName node;
            // A RPC may have multiple calls, this error may be different from
            // controller->ErrorCode();
            int error_code;
            // The controller for the RPC. Should NOT be saved in Feedback()
            // and used after the function.
            // const Controller* controller;
        };
        virtual ~LoadBalancer() = default;
        virtual void add_server(const ServerName& servers) = 0;
        virtual void remove_server(const ServerName& servers) = 0;
        // Add a list of `servers' into this balancer.
        // Returns number of servers added.
        virtual size_t AddServersInBatch(const std::vector<ServerName>& servers) = 0;

        // Remove a list of `servers' from this balancer.
        // Returns number of servers removed.
        virtual size_t RemoveServersInBatch(const std::vector<ServerName>& servers) = 0;
        
        virtual int select_server(const SelectIn& in, SelectOut* out) = 0;
        virtual void Feedback(const CallInfo& info) = 0;
    };
}