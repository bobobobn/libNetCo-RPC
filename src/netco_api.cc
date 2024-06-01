#include "../include/netco_api.h"
#include <iostream>


void netco::co_go(std::function<void()>& func, size_t stackSize, int tid)
{
    using namespace netco;
    Scheduler* myScheduler = Scheduler::getScheduler(parameter::threadNum);
    if(tid < 0)
    {
        myScheduler->createNewCo(func, stackSize);
    }
    else
    {
        myScheduler->createNewCo(func, stackSize, tid);
    }
}
void netco::co_go(std::function<void()>&& func, size_t stackSize, int tid)
{
    using namespace netco;
    Scheduler* myScheduler = Scheduler::getScheduler(parameter::threadNum);
    if(tid < 0)
    {
        myScheduler->createNewCo(func, stackSize);
    }
    else
    {
        myScheduler->createNewCo(func, stackSize, tid);
    }
}

void netco::co_sleep(int64_t ms)
{
    using namespace netco;
    Scheduler* myScheduler = Scheduler::getScheduler(parameter::threadNum);
    myScheduler->getProcessor(threadIdx)->sleep(ms);
}

void netco::sche_join()
{
    using namespace netco;
    Scheduler* myScheduler = Scheduler::getScheduler(parameter::threadNum);
    myScheduler->join();
}