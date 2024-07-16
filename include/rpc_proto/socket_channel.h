#pragma once
#include <memory>
#include <string>
#include <functional>
namespace netco{
    // 虚基类SocketChannel,后续用于定义短连接、连接池、单链接SocketChannel
    class SocketChannel{
        public:
            using UniquePtr = std::unique_ptr<SocketChannel>;
            using NewFn = std::function<SocketChannel::UniquePtr(std::string)>;
            virtual ~SocketChannel(){};
            virtual int send(const void* data, size_t len) = 0;
            virtual int recv(void* data, size_t len) = 0;
            virtual int recv_timeout(void* data, size_t len, int timeout_ms) = 0;
            virtual int close() = 0;
            virtual bool is_connected() const = 0;
    };

}