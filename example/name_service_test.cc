#include <iostream>
#include <unistd.h>
#include "../include/rpc_proto/zk_naming_service.h"

using namespace netco;
int main() {
    auto ns = ZKNamingService::New();
    while(1){
        auto data = ns->GetServerListPtr("test", "factorial");
        if(data){
            for(auto&it : **data){
                std::cout <<"server: " <<it.ipPort_.c_str() << std::endl;
            }

        }
        sleep(1);
    }

    return 0;
}