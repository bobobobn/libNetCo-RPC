#include "../include/processor_selector.h"

using namespace netco;

ProcessorPtr ProcessorSelector::next()
{
    size_t proCnt = processors_.size();
    if( 0 == proCnt)
    {
        return nullptr;
    }
    ProcessorPtr ret = nullptr;

    switch(strategy_){
        case ROUND_ROBIN:
            ret = processors_[++curPro];
            break;
        case MIN_EVENT_FIRST:
            {
                size_t minCoCnt = ~0U;
                for(int i = 0; i < proCnt; i++)
                {
                    size_t coCnt = processors_[i]->getCoCnt();
                    if( coCnt < minCoCnt)
                    {
                        minCoCnt = coCnt;
                        ret = processors_[i];
                    }
                }
                break;
            }
        default:
            break;
    }
    return ret;
}