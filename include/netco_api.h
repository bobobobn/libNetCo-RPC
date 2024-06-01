#pragma once
#include "scheduler.h"
#include "mstime.h"
#include "parameter.h"

namespace netco
{
	void co_go(std::function<void()>& func, size_t stackSize = parameter::coroutineStackSize, int tid = -1);
	void co_go(std::function<void()>&& func, size_t stackSize = parameter::coroutineStackSize, int tid = -1);

	void co_sleep(int64_t ms);

	void sche_join();
}
