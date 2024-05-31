#include "../include/processor.h"
#include "../include/objpool.h"
#include <iostream>
#include <cassert>
#include <memory>
using namespace netco;

__thread int threadIdx;

static void* threadLoop(void* processor)
{
    Processor* pro = (Processor*) processor;
    pro->eventLoop();
}



Processor::Processor(int tid)
    : tid_(tid), curCo_(nullptr), thread_(nullptr), runningQueue_(0), mainCtx_(0), pStatus_(PRO_STOPPING)
{    
    mainCtx_.makeCurContext();
}

Processor::~Processor()
{
    if( PRO_RUNNING == pStatus_ )
    {
        stop();
    }
    if( PRO_STOPPED == pStatus_ )
    {
        join();
    }
    {    
        GuardLocker lock(coSetLocker);
        for( Coroutine* co : coSet_)
        {
            GuardLocker lock(objPoolLocker_);
            objPool_.delete_obj(co);
        }
    }

}

void Processor::eventLoop()
{
    threadIdx = tid_;
    while( PRO_RUNNING == pStatus_ )
    {
        if(actCoroutines_.size())
            actCoroutines_.clear();
        if(timerExpiredCo_.size())
            timerExpiredCo_.clear();
        epoller_.getActEvServ(-1 /*parameter::epollTimeOutMs*/, actCoroutines_);
        timer_.getExpiredCoroutines(timerExpiredCo_);

        // 恢复超时协程
        for(Coroutine* expiredCo:timerExpiredCo_){
            int alive_flag = 0;
            {
                GuardLocker lock(coSetLocker);
                if(coSet_.find(expiredCo) != coSet_.end()){
                    alive_flag = 1;
                }
            }
            if(alive_flag)
            {
                resume(expiredCo);
            }
        }
        // 启动新加入的协程
        while(!newCoroutines_[runningQueue_].empty())
        {
            Coroutine* co;
            co = newCoroutines_[runningQueue_].front();
            newCoroutines_[runningQueue_].pop_front();
            {
                GuardLocker lock(coSetLocker);
                coSet_.insert(co);
            }
            printf("coID:%d starting\n", co->id_);
            resume(co);
            
        }
        {
            GuardLocker lock(queueLocker_);
            runningQueue_ = !runningQueue_;
        }

        // 恢复事件协程
        for(Coroutine* activeCo:actCoroutines_){
            resume(activeCo);
        }

        // 删除退出的协程，归还内存块
        for(Coroutine* deadCo:removedCo_){
            {
                GuardLocker lock(coSetLocker);
                coSet_.erase(deadCo);
            }
            {
                GuardLocker lock(objPoolLocker_);
                objPool_.delete_obj(deadCo);
            }
        }
        removedCo_.clear();
    }
    pStatus_ = PRO_STOPPED;
    
}

void Processor::loop()
{
    if(!epoller_.init())
        return;
    if(!timer_.init(&epoller_))
        return;
    pStatus_ = PRO_RUNNING;
    thread_ = std::make_unique<pthread_t>();
    int ret = pthread_create(&*thread_, nullptr, threadLoop, this);
    assert(ret==0);
    
}

void Processor::killCurCo()
{
	removedCo_.push_back(curCo_);
}

void Processor::resume(Coroutine* co)
{
    if(nullptr == co)
        return;
    curCo_ = co;
    co->resume();
}

void Processor::yield()
{
    printf("coID:%d yield\n", curCo_->id_);
    curCo_->yield();
    mainCtx_.swapToMe(curCo_->getCtx());
}

void Processor::sleep(int64_t ms)
{
    timer_.runAfter(Time(ms), curCo_);
    yield();
}

void Processor::waitEvent(int fd, int event)
{
    epoller_.addEv(curCo_, fd, event);
    yield();
    epoller_.removeEv(curCo_, fd, event);
}

void Processor::goNewCo(std::function<void()>&& coFuncb, size_t stackSize)
{
    Coroutine* newCo = nullptr;
    {
        GuardLocker lock(objPoolLocker_);
        newCo = objPool_.new_obj(this, stackSize, coFuncb);
    }
    {
        GuardLocker lock(queueLocker_);
        newCoroutines_[runningQueue_].push_back(newCo);
    }
    timer_.wakeUp();
}

void Processor::goNewCo(std::function<void()>& coFuncb, size_t stackSize)
{
    Coroutine* newCo = nullptr;
    {
        GuardLocker lock(objPoolLocker_);
        newCo = objPool_.new_obj(this, stackSize, coFuncb);
    }
    {    
        GuardLocker lock(queueLocker_);
        newCoroutines_[runningQueue_].push_back(newCo);
    }
    timer_.wakeUp();
}

void Processor::stop()
{
    pStatus_ = PRO_STOPPING;
    timer_.wakeUp();
}

void Processor::join()
{
    pthread_join(*thread_, nullptr);
}

size_t Processor::getCoCnt()
{
    {
        GuardLocker lock(coSetLocker);
        return coSet_.size();
    }

}