#include "../include/timer.h"
#include "../include/epoller.h"
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <cstring>
using namespace netco;

Timer::Timer()
    :timeFd_(-1)
{}

Timer::~Timer(){
    if(isTimeFdUseful())
        ::close(timeFd_);
}

bool Timer::init(Epoller* epoller)
{
    timeFd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(isTimeFdUseful()){
        return epoller->addEv(nullptr, timeFd_, EPOLLIN | EPOLLPRI | EPOLLRDHUP);
    }
    return false;
}

void Timer::getExpiredCoroutines(std::vector<Coroutine*>& expiredCoroutines)
{
    Time now = Time::now();
    while( (!timerCoHeap_.empty()) && (timerCoHeap_.top().first < now) ){
        Coroutine* expiredCoroutine = timerCoHeap_.top().second;
        expiredCoroutines.push_back(expiredCoroutine);
        timerCoHeap_.pop();
    }
    ssize_t cnt = TIMER_DUMMYBUF_SIZE;
    while (cnt >= TIMER_DUMMYBUF_SIZE)
    {
        cnt = ::read(timeFd_, dummyBuf_, TIMER_DUMMYBUF_SIZE);
    }
    if( (!timerCoHeap_.empty()) ){
		Time time = timerCoHeap_.top().first;
		resetTimeOfTimefd(time);
    }
}

bool Timer::resetTimeOfTimefd(Time time){
    struct itimerspec newValue;
	memset(&newValue, 0, sizeof newValue);
    newValue.it_value = time.timeIntervalFromNow();
    int ret = ::timerfd_settime(timeFd_, 0, &newValue, nullptr);
    return ret<0 ? false : true;
}

void Timer::runAt(Time time, Coroutine* co)
{
    timerCoHeap_.push({time, co});
    if(timerCoHeap_.top().first == time)
    {
        resetTimeOfTimefd(time);
    }
}

void Timer::runAfter(Time time, Coroutine* co)
{
    Time afterTime(Time::now().getTimeVal() + time.getTimeVal());
    runAt(afterTime, co);
}

void Timer::wakeUp()
{
    resetTimeOfTimefd(Time::now());
}