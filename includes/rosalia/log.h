#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*TODO api? whats the intended usage/pattern here?
- need opt-in thread safety across single logger
- backends for writing to files, printing, in-memory cycle buffer, //TODO composable??
- debug log statements that compile out of release builds, but can be on demand disabled


usage examples:
logger* log;
LOG_CREATE(&log);
LOG_DESTROY(&log);
LOGF(log, LOGL_INFO, "thing");
LOGF_FATAL(log, "thing")

*/

typedef enum LOGL_E {
    LOGL_NONE = 0,

    LOGL_DEBUG,
    LOGL_INFO,
    LOGL_WARN,
    LOGL_ERROR,
    LOGL_FATAL,

    LOGL_COUNT,
    LOGL_SIZE_MAX = UINT8_MAX,
} LOGL;

#ifdef __cplusplus
}
#endif
