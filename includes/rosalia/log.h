#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum __attribute__((__packed__)) ROSA_LOG_TYPE_E {
    ROSA_LOG_TYPE_NONE = 0,

    ROSA_LOG_TYPE_DEBUG,
    ROSA_LOG_TYPE_INFO,
    ROSA_LOG_TYPE_WARN,
    ROSA_LOG_TYPE_ERROR,
    ROSA_LOG_TYPE_FATAL,

    ROSA_LOG_TYPE_COUNT,
    ROSA_LOG_TYPE_SIZE_MAX = UINT8_MAX,
} ROSA_LOG_TYPE;

#ifdef __cplusplus
}
#endif
