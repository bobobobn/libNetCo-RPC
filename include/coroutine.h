#pragma once
#include "context.h"
#include <functional>


namespace netco{
    class Processor;

    /* 协程类Coroutine, 由Processor接管, 并由Processor调度 */
    class Coroutine{
    public:
        enum coStatus
        {
            CO_READY = 0,
            CO_RUNNING,
            CO_WAITING,
            CO_DEAD,
        };
        typedef std::function<void()> coFunCallBack;
        Coroutine(Processor*, size_t stackSize, coFunCallBack&&);
		Coroutine(Processor*, size_t stackSize, coFunCallBack&);
        ~Coroutine();
        void resume();
        void yield();
        inline void startFunc() { coFuncb_(); }
        inline Context* getCtx() { return &ctx_; }
        int id_;
    private:
        coFunCallBack coFuncb_;
        Context ctx_;
        Processor* pMyProcessor_;
        coStatus status_;
        static int coId_;
    };

}