#pragma once
#include <map>
#include <queue>
#include <vector>
#include <mutex>
#include <functional>
#include <time.h>
#include "mstime.h"

#define TIMER_DUMMYBUF_SIZE 1024

namespace netco
{
    class Coroutine;
    class Epoller;

    class Timer
    {
    public:
        using TimerHeap = typename std::priority_queue<
        std::pair<Time, Coroutine*>,
        std::vector<std::pair<Time, Coroutine*>>,
        std::greater<std::pair<Time, Coroutine*>>
        >;
        Timer();
        ~Timer();

        /** 定时器初始化*/
		bool init(Epoller*);
        /** 返回超时协程任务队列*/
		void getExpiredCoroutines(std::vector<Coroutine*>& expiredCoroutines);

		/** 在time时刻运行协程*/
		void runAt(Time time, Coroutine* pCo);

		/** 经过time时刻运行协程*/
		void runAfter(Time time, Coroutine* pCo);

		/** 唤醒epoll控制器*/
		void wakeUp();

	private:

		/** 使用绝对时刻fd设置timefd的到期时间*/
		bool resetTimeOfTimefd(Time time);

		/** 判断timefd是否可用*/
		inline bool isTimeFdUseful() { return timeFd_ < 0 ? false : true; };

		/** 定时器fd*/
		int timeFd_;

		char dummyBuf_[TIMER_DUMMYBUF_SIZE];

		/** 定时器保存的超时协程堆 {时间-协程}*/
		TimerHeap timerCoHeap_;

    };
}