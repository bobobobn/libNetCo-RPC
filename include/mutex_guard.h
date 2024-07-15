#pragma once
#include <pthread.h>
#include "utils.h"

namespace netco {
	class MutexLock;
	class MutexGuard
	{
	public:
		MutexGuard(MutexLock& l)
			: lock_(l)
		{
			lock_.lock();
		}

		~MutexGuard()
		{
			lock_.unlock();
		}

		DISALLOW_COPY_MOVE_AND_ASSIGN(MutexGuard);

	private:
		MutexLock& lock_;

	};

	class MutexLock{
		public:
			MutexLock()
			{
				pthread_mutex_init(&lock_, NULL);
			}
			~MutexLock()
			{
				pthread_mutex_destroy(&lock_);
			}
			void lock()
			{
				pthread_mutex_lock(&lock_);
			}
			void unlock()
			{
				pthread_mutex_unlock(&lock_);
			}
			
		private:
			pthread_mutex_t lock_;
	};
	

}
