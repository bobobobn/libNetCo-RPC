#pragma once

#include <atomic>
#include "netco_api.h"


namespace netco {
    extern __thread int threadIdx;
    class CoMutex {
    public:
        void lock() {
            while (locked.test_and_set(std::memory_order_acquire)) {
                netco::co_sleep(0);
            }
        }
        void unlock() {
            locked.clear(std::memory_order_release);
        }
    private:
        std::atomic_flag locked = ATOMIC_FLAG_INIT;
    };
    class GuardCoMutex{
    public:
        GuardCoMutex(CoMutex& mutex) : m_mutex(mutex) {
            m_mutex.lock();
        }
        ~GuardCoMutex() {
            m_mutex.unlock();
        }
    private:
        CoMutex& m_mutex;
    };

}