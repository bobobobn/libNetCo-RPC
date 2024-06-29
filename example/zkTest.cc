#include "../include/zk_client.h"
#include "../include/log.h"
#include "../include/parameter.h"

int main() {
    using namespace netco;
    ZkClient client(parameter::zkServerAddr);
    client.create("/test", "hello", ZOO_EPHEMERAL);
    std::string value = client.get("/test");
    NETCO_LOG()<<"length: "<<value.length()<<",value: "<<value;

    ZkClient client2(parameter::zkServerAddr);
    std::string value2 = client.get("/test");
    NETCO_LOG()<<"length: "<<value2.length()<<",value: "<<value2;
    while(1);
    return 0;
}   