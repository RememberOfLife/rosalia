#pragma once

#include <stdint.h>

#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

const static rosa_semver rosa_timestamp_version = (rosa_semver){0, 2, 0};

//TODO work through https://github.com/floooh/sokol/blob/master/sokol_time.h

// general purpose timing function that counts up monotonically
uint64_t rosa_timestamp_get_ms64(); // milliseconds
uint64_t rosa_timestamp_get_ns64(); // nanoseconds

static const uint32_t ROSA_TIMESTAMP_MAX_FRACTION = 999999999;

// represents either: a point in time, if precision allows, to the nanosecond
// or: a duration of time
typedef struct rosa_timestamp_s {
    uint64_t time; // unix timestamp in seconds
    uint32_t fraction; // nanosecond fraction 0-999.999.999 (TIMESTAMP_MAX_FRACTION)
} rosa_timestamp;

rosa_timestamp rosa_timestamp_get();
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
int rosa_timestamp_compare(rosa_timestamp lhs, rosa_timestamp rhs);

// returns the absolute difference between ts1 and ts2, regardless of which is greater, as a duration
rosa_timestamp rosa_timestamp_diff(rosa_timestamp ts1, rosa_timestamp ts2);

// recommended use: timestamp + duration
rosa_timestamp rosa_timestamp_add(rosa_timestamp ts1, rosa_timestamp ts2);

#ifdef __cplusplus
}
#endif

//TODO serialization.h compatibility and timestamp serialization layout
// extern const serialization_layout sl_timestamp[];
// const serialization_layout sl_timestamp[] = {
//     {SL_TYPE_U64, offsetof(timestamp, time)},
//     {SL_TYPE_U32, offsetof(timestamp, fraction)},
//     {SL_TYPE_STOP},
// };
