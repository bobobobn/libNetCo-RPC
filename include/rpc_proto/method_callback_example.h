#pragma once

#include <functional>
#include <string>
#include "int_message.pb.h"
#include "../log.h"
namespace netco{    
    // callback function for factorial method
    std::string factorial(const std::string& message){
        NETCO_LOG()<<"entering factorial callback function";
        IntMessage request;
        request.ParseFromString(message);
        NETCO_LOG_FMT("factorial called with message: %d!", request.value());
        IntMessage response;
        response.set_value(1);
        for(int i=1; i<=request.value(); i++){
            response.set_value(response.value() * i);
        }
        NETCO_LOG_FMT("factorial result: %d", response.value());
        return response.SerializeAsString();
    }
}