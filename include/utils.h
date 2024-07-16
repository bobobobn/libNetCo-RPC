#pragma once
#include <sys/time.h>
#include <cstdint>
#include <cstddef>
#define DISALLOW_COPY_MOVE_AND_ASSIGN(TypeName) TypeName(const TypeName&) = delete; TypeName(const TypeName&&) = delete;  TypeName& operator=(const TypeName&) = delete

namespace netco{

namespace utils{

    inline int64_t gettimeofday_us() {
        timeval now;
        ::gettimeofday(&now, NULL);
        return now.tv_sec * 1000000L + now.tv_usec;
    }
}
}
