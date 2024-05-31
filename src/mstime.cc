#include "../include/mstime.h"
#include <sys/time.h>
#include <cstring>


using namespace netco;

Time Time::now(){
    timeval now;
    ::gettimeofday(&now, 0);
    return Time(now.tv_sec*1000 + now.tv_usec/1000);
}

struct timespec Time::timeIntervalFromNow()
{
	struct timespec ts;
    memset(&ts, 0, sizeof(timespec));
	int64_t microseconds = timeVal_ - Time::now().getTimeVal();
	if (microseconds < 1)
	{
		ts.tv_sec = 0;
		ts.tv_nsec = 1000;
	}
	else 
	{
		ts.tv_sec = static_cast<time_t>(
			microseconds / 1000);
		ts.tv_nsec = static_cast<long>(
			(microseconds % 1000) * 1000 * 1000);
	}
	return ts;
}