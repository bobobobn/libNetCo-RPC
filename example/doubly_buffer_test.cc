#include "../include/container/doubly_buffered_data.h"
#include <pthread.h>
#include <iostream>
#include <atomic>
#include <unistd.h>

using namespace std;

namespace netco{
__thread int threadIdx;
}

size_t Add(int& data){
    data++;
    return 0;
}
std::atomic<int> counter(0);
void* reading(void* arg){
    using namespace netco;
    threadIdx = counter.fetch_add(1);
    DoublyBufferedData<int>* dbd = static_cast<DoublyBufferedData<int>*>(arg);
    if(threadIdx == 0){
        while(true){
            sleep(1);
            dbd->Modify(Add);
        }
    }
    while(true){
        DoublyBufferedData<int>::ScopePtr ptr;
        dbd->Read(ptr);
        cout << "thread: "<< threadIdx << " reading: " << *ptr << endl;
    }
}
int main() {
    using namespace netco;
    DoublyBufferedData<int> dbd;
    const int threadNum = 10;
    pthread_t threads[threadNum];
    for(int i=0; i<threadNum; i++){
        pthread_create(&threads[i], NULL, reading, (void *)(&dbd));
    }

    for(int i=0; i<threadNum; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
