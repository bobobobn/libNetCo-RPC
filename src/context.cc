#include "../include/context.h"
#include <cstdlib>

using namespace netco;

Context::Context(size_t sSize)
    :pStack_(nullptr), stackSize_(sSize)
    { }

Context::~Context(){
    if(pStack_)
        free(pStack_);
}

void Context::makeCurContext(){
    ::getcontext(&ctx_);
}

void Context::makeContext(Processor* pro, void (*func)(), Context* oldCtx){
    if(nullptr == pStack_)
    {
        pStack_ = malloc(stackSize_);
    }
    ::getcontext(&ctx_);
    ctx_.uc_stack.ss_sp = pStack_;
    ctx_.uc_stack.ss_size = stackSize_;
    ctx_.uc_link = oldCtx->getCtx();
    ::makecontext(&ctx_, func, 1, pro);
}

void Context::swapToMe(Context* oldCtx){
    if(nullptr == oldCtx){
        ::setcontext(&ctx_);
    }
    else
    {
        ::swapcontext(oldCtx->getCtx(), &ctx_);
    }
}