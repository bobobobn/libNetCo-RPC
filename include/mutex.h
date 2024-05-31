#pragma once
#include <pthread.h>
#include <bits/codecvt.h>

namespace netco{
    class MutexLock
    {
    public:
        MutexLock()
        {
            pthread_mutex_init(&mutex_, NULL);
        }
        ~MutexLock() {}
        void lock()
        {
            pthread_mutex_lock(&mutex_);
        }
        void unlock()
        {
            pthread_mutex_unlock(&mutex_);
        }
    private:
        pthread_mutex_t mutex_;
    };

    class GuardLocker{
    public:
        GuardLocker(MutexLock mutexLocker)
            :mutex_(mutexLocker)
        {
            mutexLocker.lock();
        }
        ~GuardLocker()
        {
            mutex_.unlock();
        }
    private:
        MutexLock mutex_;
    };
    #define GuardLocker(x) error "Missing guard object name"
}