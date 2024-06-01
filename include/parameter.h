#pragma once
#include <stddef.h>

namespace netco
{
	namespace parameter
	{
		//协程的调用栈大小
		const static size_t coroutineStackSize = 32 * 1024;

		// 存储epoll活动事件的vector初始大小
		static constexpr int epollerListFirstSize = 16;

		// epoll_wait等待时长
		static constexpr int epollTimeOutMs = 10000;

		// socket listen的backLog参数
		constexpr static unsigned backLog = 4096;

		// 内存池分配对象大小的内存个数
		static constexpr size_t memPoolMallocObjCnt = 40;

		// 默认线程数，为NULL时，线程数=CPU核心数
		static constexpr int threadNum = NULL;
	}
	
}