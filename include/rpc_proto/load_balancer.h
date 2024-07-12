#pragma once

namespace netco{
    class LoadBalancer{
        virtual void add_server() = 0;
        virtual void remove_server() = 0;
        virtual void select_server() = 0;
    };
}