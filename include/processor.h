#pragma once
#include "epoller.h"
#include "timer.h"
#include "coroutine.h"
#include "mutex.h"
#include "context.h"
#include "parameter.h"
#include "objpool.h"
#include <pthread.h>
#include <vector>
#include <queue>
#include <list>
#include <memory>
#include <set>
#include <thread>


extern __thread int threadIdx;
namespace netco
{   
    /*
        运行着one thread one loop的processor类,
        管理coroutine对象,
        thread的loop为eventLoop函数
    */
    class Processor
    {
    public:
    	enum processerStatus
        {
            PRO_RUNNING = 0,
            PRO_STOPPING,
            PRO_STOPPED
        };
        Processor(int);
        ~Processor();
        void loop();
        void resume(Coroutine* co);
        void yield();
        void sleep(int64_t);
        void eventLoop();
        inline Context* getMainCtx() { return &mainCtx_; }
        inline Coroutine* getCurCo() { return curCo_; }
        void killCurCo();
        void goNewCo(std::function<void()>&, size_t stackSize);
        void goNewCo(std::function<void()>&&, size_t stackSize);
        void waitEvent(int fd, int event);
        void stop();
        void join();
        size_t getCoCnt();


    private:
        Epoller epoller_;
        Timer timer_;
        MutexLock queueLocker_;
        MutexLock objPoolLocker_;
        MutexLock coSetLocker;
        Coroutine* curCo_;
        Context mainCtx_;
        int tid_;

        std::unique_ptr<pthread_t> thread_;
        int runningQueue_;
        processerStatus pStatus_;
        std::list<Coroutine*> newCoroutines_[2];
		std::vector<Coroutine*> actCoroutines_;
		std::vector<Coroutine*> timerExpiredCo_;
        std::vector<Coroutine*> removedCo_;
        std::set<Coroutine*> coSet_;

        objPool<Coroutine> objPool_;
    };
    using ProcessorPtr = typename std::shared_ptr<Processor>;
}