#pragma once
#include "socket_channel.h"
#include "../socket.h"
#include <string>

namespace netco{
    // 短连接的socket channel
    class ShortSocketChannel : public SocketChannel{
    public:
        ShortSocketChannel(std::string server_addr):_sock_fd(), _connected(false){    
            if (server_addr.empty()){
            // NETCO_LOG_ERROR(NETCO_LOG_ROOT())<<"RpcClientStub::call: server_addr is empty";
                return;
            }
            size_t pos = server_addr.find(":");
            if (pos == std::string::npos){
                // NETCO_LOG_ERROR(NETCO_LOG_ROOT())<<"RpcClientStub::call: server_addr format error";
                return;
            }
            std::string ip = server_addr.substr(0, pos);
            int port = std::stoi(server_addr.substr(pos + 1));
            int ret = _sock_fd.connect(ip.c_str(), port);
            _connected = ret==0;
        }
        ~ShortSocketChannel() override{
            _connected = false;
            close();
        }
        int send(const void* data, size_t len) override {
            return _sock_fd.send(data, len);
        }
        int recv(void* data, size_t len) override {
            return _sock_fd.read(data, len);
        }
        int recv_timeout(void* data, size_t len, int timeout_ms) override { return 0;}
        int close() override {
            _sock_fd.shutdownWrite();
            return 0;
        }
        bool is_connected() const override{
            return _connected;
        }
        static SocketChannel::UniquePtr New(std::string server_addr){
            return std::make_unique<ShortSocketChannel>(server_addr);
        }
    private:
        bool _connected;
        Socket _sock_fd;
    };
}