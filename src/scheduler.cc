#include "../include/scheduler.h"
#include <cassert>
using namespace netco;

const int Scheduler::procNums = ::get_nprocs_conf();
Scheduler::Scheduler(int threadCnt)
    :proSelector_(processors_, MIN_EVENT_FIRST)
{
    assert(threadCnt>0);
    for(int i = 0; i < threadCnt; i++){
        ProcessorPtr myPro = std::make_shared<Processor>(i);
        myPro->loop();
        processors_.push_back(std::move(myPro));
    }
}

Scheduler* Scheduler::getScheduler(int threadCnt)
{
    static Scheduler mySheduler(threadCnt);
    return &mySheduler;
}

void Scheduler::createNewCo(std::function<void()>&& func, size_t stackSize)
{
    proSelector_.next()->goNewCo(func, stackSize);
}

void Scheduler::createNewCo(std::function<void()>& func, size_t stackSize)
{
    proSelector_.next()->goNewCo(func, stackSize);
}

void Scheduler::createNewCo(std::function<void()>&& func, size_t stackSize, int tid)
{
    processors_[tid % processors_.size() ]->goNewCo(func, stackSize);
}

void Scheduler::createNewCo(std::function<void()>& func, size_t stackSize, int tid)
{
    processors_[tid % processors_.size() ]->goNewCo(func, stackSize);
}

void Scheduler::join()
{
	for (auto pP : processors_)
	{
		pP->join();
	}
}

ProcessorPtr Scheduler::getProcessor(int id)
{
	return processors_[id];
}

