#pragma once
#include <stddef.h>
#include <sys/sysinfo.h>

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
		static const int defaultThreadNum = ::get_nprocs_conf();
		// static const int defaultThreadNum = 1;
	}
	
}