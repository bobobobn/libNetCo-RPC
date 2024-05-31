#include "../include/netco_api.h"
#include <iostream>

using namespace netco;

NetCo* NetCo::getNetCo(int threadNum)
{
    static NetCo myNetCo(threadNum);
    return &myNetCo;
}
NetCo::NetCo(int threadNum)
    : threadNum_(threadNum)
{
    myScheduler_ = Scheduler::getScheduler(threadNum_);
}
NetCo::~NetCo()
{}
void NetCo::co_go(std::function<void()>& func, size_t stackSize, int tid) const
{
    if(tid < 0)
    {
        myScheduler_->createNewCo(func, stackSize);
    }
    else
    {
        myScheduler_->createNewCo(func, stackSize, tid);
    }
}
void NetCo::co_go(std::function<void()>&& func, size_t stackSize, int tid) const{
    if(tid < 0)
    {
        myScheduler_->createNewCo(func, stackSize);
    }
    else
    {
        myScheduler_->createNewCo(func, stackSize, tid);
    }
}

void NetCo::co_sleep(int64_t ms) const
{    
    myScheduler_->getProcessor(threadIdx)->sleep(ms);
}

void NetCo::sche_join() const
{
    myScheduler_->join();
}