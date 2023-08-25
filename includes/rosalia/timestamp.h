#ifndef ROSALIA_TIMESTAMP_H_INCLUDE
#define ROSALIA_TIMESTAMP_H_INCLUDE

#include <stdint.h>

#ifdef ROSALIA_TIMESTAMP_STATIC
#define ROSALIA__TIMESTAMP_DEC static
#define ROSALIA__TIMESTAMP_DEC_EXT static
#define ROSALIA__TIMESTAMP_DEF static
#else
#define ROSALIA__TIMESTAMP_DEC
#define ROSALIA__TIMESTAMP_DEC_EXT extern
#define ROSALIA__TIMESTAMP_DEF
#endif

#ifdef ROSALIA_TIMESTAMP_DECORATE
#define ROSALIA__TIMESTAMP_DECORATE(ident) ROSALIA_TIMESTAMP_DECORATE(ident)
#else
#define ROSALIA__TIMESTAMP_DECORATE(ident) ident
#endif

#define ROSALIA_TIMESTAMP_VERSION_MAJOR 0
#define ROSALIA_TIMESTAMP_VERSION_MINOR 1
#define ROSALIA_TIMESTAMP_VERSION_PATCH 4

#ifdef __cplusplus
extern "C" {
#endif

//TODO work through https://github.com/floooh/sokol/blob/master/sokol_time.h

// general purpose timing function that counts up monotonically
ROSALIA__TIMESTAMP_DEC uint64_t ROSALIA__TIMESTAMP_DECORATE(timestamp_get_ms64)(); // milliseconds
ROSALIA__TIMESTAMP_DEC uint64_t ROSALIA__TIMESTAMP_DECORATE(timestamp_get_ns64)(); // nanoseconds

static const uint32_t TIMESTAMP_MAX_FRACTION = 999999999;

//TODO decorate struct
// represents either: a point in time, if precision allows, to the nanosecond
// or: a duration of time
typedef struct timestamp_s {
    uint64_t time; // unix timestamp in seconds
    uint32_t fraction; // nanosecond fraction 0-999.999.999 (TIMESTAMP_MAX_FRACTION)
} timestamp;

ROSALIA__TIMESTAMP_DEC timestamp ROSALIA__TIMESTAMP_DECORATE(timestamp_get)();
/*
// usage example to extract local time info from this:
timestamp ts = timestamp_get(NULL);
struct tm* info;
info = localtime((const time_t*)&ts.time);
printf("%d-%d-%d %02d:%02d:%02d.%09d", 1900 + info->tm_year, info->tm_mon, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec, ts.fraction);
*/

// lhs < rhs ==> -1
// lhs = rhs ==> 0
// lhs > rhs ==> 1
ROSALIA__TIMESTAMP_DEC int ROSALIA__TIMESTAMP_DECORATE(timestamp_compare)(timestamp lhs, timestamp rhs);

// returns the absolute difference between ts1 and ts2, regardless of which is greater, as a duration
ROSALIA__TIMESTAMP_DEC timestamp ROSALIA__TIMESTAMP_DECORATE(timestamp_diff)(timestamp ts1, timestamp ts2);

// recommended use: timestamp + duration
ROSALIA__TIMESTAMP_DEC timestamp ROSALIA__TIMESTAMP_DECORATE(timestamp_add)(timestamp ts1, timestamp ts2);

#ifdef __cplusplus
}
#endif

#endif

//TODO serialization.h compatibility and timestamp serialization layout
// extern const serialization_layout sl_timestamp[];
// const serialization_layout sl_timestamp[] = {
//     {SL_TYPE_U64, offsetof(timestamp, time)},
//     {SL_TYPE_U32, offsetof(timestamp, fraction)},
//     {SL_TYPE_STOP},
// };

#if defined(ROSALIA_TIMESTAMP_IMPLEMENTATION) && !defined(ROSALIA_TIMESTAMP_H_IMPL)
#define ROSALIA_TIMESTAMP_H_IMPL

#define ROSALIA__TIMESTAMP_INTERNAL(ident) rosalia__timestamp_internal_##ident

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

ROSALIA__TIMESTAMP_DEF uint64_t ROSALIA__TIMESTAMP_DECORATE(timestamp_get_ms64)()
{
    struct timespec record;
    clock_gettime(CLOCK_MONOTONIC_RAW, &record);
    return record.tv_sec * 1000 + record.tv_nsec / 1000000;
}

ROSALIA__TIMESTAMP_DEF uint64_t ROSALIA__TIMESTAMP_DECORATE(timestamp_get_ns64)()
{
    struct timespec record;
    clock_gettime(CLOCK_MONOTONIC_RAW, &record);
    return record.tv_sec * 1000000000 + record.tv_nsec;
}

// modelled after: https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/timestamp.proto

ROSALIA__TIMESTAMP_DEF timestamp ROSALIA__TIMESTAMP_DECORATE(timestamp_get)()
{
    timestamp ret;
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

ROSALIA__TIMESTAMP_DEF int ROSALIA__TIMESTAMP_DECORATE(timestamp_compare)(timestamp lhs, timestamp rhs)
{
    int rv = (lhs.time > rhs.time) - (lhs.time < rhs.time);
    if (rv == 0) {
        rv = (lhs.fraction > rhs.fraction) - (lhs.fraction < rhs.fraction);
    }
    return rv;
}

ROSALIA__TIMESTAMP_DEF timestamp ROSALIA__TIMESTAMP_DECORATE(timestamp_diff)(timestamp ts1, timestamp ts2)
{
    timestamp res;
    int64_t fraction;
    int cmp = ROSALIA__TIMESTAMP_DECORATE(timestamp_compare)(ts1, ts2);
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
            fraction += (int64_t)TIMESTAMP_MAX_FRACTION + 1;
        }
    }
    res.fraction = (uint32_t)fraction;
    return res;
}

ROSALIA__TIMESTAMP_DEF timestamp ROSALIA__TIMESTAMP_DECORATE(timestamp_add)(timestamp ts1, timestamp ts2)
{
    timestamp res = (timestamp){
        .time = ts1.time + ts2.time,
        .fraction = ts1.fraction + ts2.fraction,
    };
    if (res.fraction > TIMESTAMP_MAX_FRACTION) {
        res.time += 1;
        res.fraction -= TIMESTAMP_MAX_FRACTION + 1;
    }
    return res;
}

#ifdef __cplusplus
}
#endif

#endif
