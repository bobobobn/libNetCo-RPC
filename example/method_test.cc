#include <iostream>
#include "../include/rpc_proto/rpc_method.h"
#include "../include/rpc_proto/int_message.pb.h"
#include "../include/rpc_proto/method_callback_example.h"

int main(){
    netco::RpcMethod channel("factorial", &netco::factorial);
    IntMessage request;
    request.set_value(10);
    std::string request_str = request.SerializeAsString();

    IntMessage response;
    std::string response_str;
    response_str = channel.callMethod(request_str);
    response.ParseFromString(response_str);

    std::cout << "MethodTest response: " << response.value() << std::endl;

    return 0;
}