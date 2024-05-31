#pragma once

#include "processor.h"
#include "processor_selector.h"
#include "mutex.h"
#include <memory>
#include <sys/sysinfo.h>

namespace netco
{   
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