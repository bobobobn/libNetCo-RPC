#include "../include/coroutine.h"
#include "../include/processor.h"

using namespace netco;

int Coroutine::coId_ = 0;
static void coWrapFunc(Processor* myPro)
{
    myPro->getCurCo()->startFunc();
    myPro->killCurCo();

}

Coroutine::Coroutine(Processor* pro, size_t stackSize, coFunCallBack&& func)
    :pMyProcessor_(pro), ctx_(stackSize), status_(CO_DEAD), coFuncb_(std::move(func)), id_(coId_)
{
    status_ = CO_READY;
    coId_++;    
}
Coroutine::Coroutine(Processor* pro, size_t stackSize, coFunCallBack& func)
    :pMyProcessor_(pro), ctx_(stackSize), status_(CO_DEAD), coFuncb_(func), id_(coId_)
{
    status_ = CO_READY;
    coId_++;
}

Coroutine::~Coroutine()
{
    #ifdef DEBUGING
        printf("coId:%d destructing\n", id_);
    #endif
}

void Coroutine::resume()
{
    #ifdef DEBUGING
        printf("CoID%d resuming\n", id_);
    #endif
    Context* mainCtx = pMyProcessor_->getMainCtx();
    // mainCtx.makeCurContext();
    switch(status_)
    {
        case CO_READY:
            status_ = CO_RUNNING;
            ctx_.makeContext( pMyProcessor_, (void (*)(void)) coWrapFunc, mainCtx);
            ctx_.swapToMe(mainCtx);
            break;
        case CO_WAITING:
            ctx_.swapToMe(mainCtx);
            break;
        default:
            break;
    }
}

void Coroutine::yield(){
    status_ = CO_WAITING;
}