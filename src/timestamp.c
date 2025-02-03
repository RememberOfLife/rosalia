#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "rosalia/timestamp.h"

#if !defined(ISMSVC)
#if (defined(_WIN32) && !(defined(__MINGW32__) || defined(__MINGW64__)))
#define ISMSVC 1
#else
#define ISMSVC 0
#endif
#endif

#if ISMSVC
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#else
#include <time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

uint64_t rosa_timestamp_get_ms64()
{
    struct timespec record;
    clock_gettime(CLOCK_MONOTONIC_RAW, &record);
    return record.tv_sec * 1000 + record.tv_nsec / 1000000;
}

uint64_t rosa_timestamp_get_ns64()
{
    struct timespec record;
    clock_gettime(CLOCK_MONOTONIC_RAW, &record);
    return record.tv_sec * 1000000000 + record.tv_nsec;
}

// modelled after: https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/timestamp.proto

rosa_timestamp rosa_timestamp_get()
{
    rosa_timestamp ret;
#if ISMSVC
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t ticks = (((uint64_t)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    // A Windows tick is 100 nanoseconds. Windows epoch 1601-01-01T00:00:00Z
    // is 11644473600 seconds before Unix epoch 1970-01-01T00:00:00Z.
    Timestamp timestamp;
    ret.time = (ticks / 10000000) - 11644473600LL;
    ret.fraction = ((ticks % 10000000) * 100;
#else
    struct timespec record;
    clock_gettime(CLOCK_REALTIME, &record);
    ret.time = record.tv_sec;
    ret.fraction = record.tv_nsec;
#endif
    return ret;
}

int rosa_timestamp_compare(rosa_timestamp lhs, rosa_timestamp rhs)
{
    int rv = (lhs.time > rhs.time) - (lhs.time < rhs.time);
    if (rv == 0) {
        rv = (lhs.fraction > rhs.fraction) - (lhs.fraction < rhs.fraction);
    }
    return rv;
}

rosa_timestamp rosa_timestamp_diff(rosa_timestamp ts1, rosa_timestamp ts2)
{
    rosa_timestamp res;
    int64_t fraction;
    int cmp = rosa_timestamp_compare(ts1, ts2);
    if (cmp == 1) {
        // ts1 > ts2
        res.time = ts1.time - ts2.time;
        fraction = (int64_t)ts1.fraction - (int64_t)ts2.fraction;
    } else {
        // ts1 <= ts2
        res.time = ts2.time - ts1.time;
        fraction = (int64_t)ts2.fraction - (int64_t)ts1.fraction;
    }
    if (fraction < 0) {
        if (cmp == 0) {
            fraction = -fraction;
        } else {
            res.time -= 1;
            fraction += (int64_t)ROSA_TIMESTAMP_MAX_FRACTION + 1;
        }
    }
    res.fraction = (uint32_t)fraction;
    return res;
}

rosa_timestamp rosa_timestamp_add(rosa_timestamp ts1, rosa_timestamp ts2)
{
    rosa_timestamp res = (rosa_timestamp){
        .time = ts1.time + ts2.time,
        .fraction = ts1.fraction + ts2.fraction,
    };
    if (res.fraction > ROSA_TIMESTAMP_MAX_FRACTION) {
        res.time += 1;
        res.fraction -= ROSA_TIMESTAMP_MAX_FRACTION + 1;
    }
    return res;
}

#ifdef __cplusplus
}
#endif
