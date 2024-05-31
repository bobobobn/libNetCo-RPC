#pragma once
#include <sys/time.h>
#include <stdint.h>

namespace netco
{
    class Time{
    public:
        Time(int64_t msSinceEpoch) : timeVal_(msSinceEpoch) {}
        Time(const Time& time) : timeVal_(time.timeVal_) {}
        Time(const Time&& time) : timeVal_(time.timeVal_) {}
        Time& operator=(const Time& time)
		{
			timeVal_ = time.timeVal_;
			return *this;
		}
        ~Time() {}
        static Time now();
        int64_t getTimeVal() { return timeVal_; }
        struct timespec timeIntervalFromNow();
    private:
        int64_t timeVal_;

    };
    inline bool operator < (Time lhs, Time rhs)
    {
        return lhs.getTimeVal() < rhs.getTimeVal();
    }

    inline bool operator <= (Time lhs, Time rhs)
    {
        return lhs.getTimeVal() <= rhs.getTimeVal();
    }

    inline bool operator > (Time lhs, Time rhs)
    {
        return lhs.getTimeVal() > rhs.getTimeVal();
    }

    inline bool operator >= (Time lhs, Time rhs)
    {
        return lhs.getTimeVal() >= rhs.getTimeVal();
    }

    inline bool operator == (Time lhs, Time rhs)
    {
        return lhs.getTimeVal() == rhs.getTimeVal();
    }
}