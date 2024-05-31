#pragma once
#include "processor.h"

namespace netco
{
    enum scheduleStrategy
    {
        MIN_EVENT_FIRST = 0 , //�����¼�����
        ROUND_ROBIN			  //�����ַ�
    };
    class ProcessorSelector
    {
    public:

        ProcessorSelector(std::vector<ProcessorPtr>& processors, scheduleStrategy strategy)
            : processors_(processors), strategy_(strategy), curPro(0)
        {}
        ~ProcessorSelector() { }
        inline void setStrategy(scheduleStrategy strategy) { strategy_ = strategy; }
        ProcessorPtr next();
    private:
        std::vector<ProcessorPtr>& processors_;
        scheduleStrategy strategy_;
        int curPro;
    };
}