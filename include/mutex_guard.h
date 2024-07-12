#pragma once
#include <pthread.h>
#include "utils.h"

namespace netco {

	class MutexGuard
	{
	public:
		MutexGuard(pthread_mutex_t& l)
			: lock_(l)
		{
			pthread_mutex_lock(&lock_);
		}

		~MutexGuard()
		{
			pthread_mutex_unlock(&lock_);
		}

		DISALLOW_COPY_MOVE_AND_ASSIGN(MutexGuard);

	private:
		pthread_mutex_t& lock_;

	};

}
