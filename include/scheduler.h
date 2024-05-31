#pragma once

#include "processor.h"
#include "processor_selector.h"
#include "mutex.h"
#include <memory>
#include <sys/sysinfo.h>

namespace netco
{   
    /* 
        单例模式的Scheduler, 
        首次getScheduler获取实例传入threadNum值,
        创建threadNum个Processor对象/线程,
        通过proSelector_调度Processor 
    */
    class Scheduler
    {
        protected:
            Scheduler(int);
            ~Scheduler(){}
        public:
            static Scheduler* getScheduler(int threadNum);

            void createNewCo(std::function<void()>&& func, size_t stackSize);
            void createNewCo(std::function<void()>& func, size_t stackSize);
            void createNewCo(std::function<void()>&& func, size_t stackSize, int tid);
            void createNewCo(std::function<void()>& func, size_t stackSize, int tid);
            ProcessorPtr getProcessor(int id);

            inline int getProCnt() { return processors_.size(); }

            void join();
            static const int procNums;
        private:

            static MutexLock scherMtx_;

            std::vector<ProcessorPtr> processors_;

            ProcessorSelector proSelector_;

    };
}