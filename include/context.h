#pragma once
#include <ucontext.h>

namespace netco{
    class Processor;
    class Context{
    public:
        Context(size_t sSize);
        ~Context();
        void makeContext(Processor* pro, void(*func)(), Context*);
        void makeCurContext();
        void swapToMe(Context* oldCtx);
        inline ucontext_t* getCtx() {return &ctx_;}
    private:
        ucontext_t ctx_;
        void* pStack_;
        size_t stackSize_;
    };
}
