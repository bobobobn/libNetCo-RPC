#include "../include/comutex.h"
#include "../include/mutex_guard.h"
#include "../include/utils.h"
#include <pthread.h>
#include <iostream>

int count = 0;
// netco::MutexLock mutex;
// netco::CoMutex mutex;

void test()
{
    int i = 1000000;
    while(i--){
        netco::GuardCoMutex guard(mutex);
        // netco::MutexGuard guard(mutex);
    }
    int cnt = netco::Scheduler::getScheduler()->getProcessor(netco::threadIdx)->getCoCnt();
    // std::cout<<"threadIdx = "<<netco::threadIdx<<" cnt = "<<cnt<<std::endl;
    if( cnt == 1 )
        netco::Scheduler::getScheduler()->getProcessor(netco::threadIdx)->stop();
}

int main(){
    int thread_num = 1;
    int64_t start_time = netco::utils::gettimeofday_us();
    while(thread_num--)
        netco::co_go(test);
    netco::sche_join();
    int64_t end_time = netco::utils::gettimeofday_us();
    std::cout<<"time cost = "<<(end_time-start_time)/1000.0<<"ms"<<std::endl;
    std::cout<<"count = "<<count<<std::endl;
    return 0;
}