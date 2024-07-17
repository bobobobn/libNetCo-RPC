#pragma once
#include <stddef.h>

namespace netco
{
	namespace parameter
	{
		const static size_t coroutineStackSize = 128 * 1024;

		static constexpr int epollerListFirstSize = 16;

		static constexpr int epollTimeOutMs = -1;

		constexpr static unsigned backLog = 4096;

		static constexpr size_t memPoolMallocObjCnt = 40;
		static const char* zkServerAddr = "127.0.0.1:2181";
	}
	
}