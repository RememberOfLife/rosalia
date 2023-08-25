#pragma once

//TODO make this single header

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*TODO api? whats the intended usage/pattern here?
- backends for writing to files, printing, in-memory cycle buffer, //TODO composable??
- debug log statements that compile out of release builds, but can be on demand disabled
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

extern const char* logl_names[LOGL_COUNT];

typedef struct rosa_logger_output_s rosa_logger_output;

// this callback will be called by a logger everytime a new log line comes up
// str end is the first character NOT part of the string, i.e. the null terminator
typedef void rosa_logger_output_push_line_t(rosa_logger_output* output, const char* str, const char* str_end);

typedef void rosa_logger_output_destroy_t(rosa_logger_output* output);

struct rosa_logger_output_s {
    uint32_t id;
    void* private_data;
    rosa_logger_output_push_line_t* push_line;
    rosa_logger_output_destroy_t* destroy;
};

typedef struct rosa_logger_s {
    char* log_name;
    char* pre_layout;
    char* post_layout;
    bool concurrent_usage;
    //TODO child loggers?
    uint32_t next_output_id;
    rosa_logger_output* outputs; // rosa_vec<rosa_logger_output>
} rosa_logger;

void rosa_log_create(rosa_logger** log, const char* log_name, bool concurrent_usage, const char* pre_layout, const char* post_layout);

void rosa_log_destroy(rosa_logger** log);

uint32_t rosa_log_output_add(rosa_logger* log, rosa_logger_output output);

bool rosa_log_output_remove(rosa_logger* log, uint32_t output_id, rosa_logger_output* output);

size_t rosa__log_internal_layout(char* buf, size_t len, rosa_logger* log, const char* layout);

void rosa__log_internal_log(rosa_logger* log, LOGL lvl, const char* str, const char* str_end);

void rosa__log_internal_logf(rosa_logger* log, LOGL lvl, const char* fmt, ...);

void rosa__log_internal_logfv(rosa_logger* log, LOGL lvl, const char* fmt, va_list args);

//TODO these all should pass their file and line number to the log function

#define ROSA_LOG(log, lvl, str, str_end) (rosa__log_internal_log(log, lvl, str, str_end))
#define ROSA_LOGF(log, lvl, fmt, ...) (rosa__log_internal_logf(log, lvl, fmt, ##__VA_ARGS__))
#define ROSA_LOGFV(log, lvl, fmt, args) (rosa__log_internal_logfv(log, lvl, fmt, args))

#define ROSA_LOG_DEBUG(log, str, str_end) (rosa__log_internal_log(log, LOGL_DEBUG, str, str_end))
#define ROSA_LOG_INFO(log, str, str_end) (rosa__log_internal_log(log, LOGL_INFO, str, str_end))
#define ROSA_LOG_WARN(log, str, str_end) (rosa__log_internal_log(log, LOGL_WARN, str, str_end))
#define ROSA_LOG_ERROR(log, str, str_end) (rosa__log_internal_log(log, LOGL_ERROR, str, str_end))
#define ROSA_LOG_FATAL(log, str, str_end) (rosa__log_internal_log(log, LOGL_FATAL, str, str_end))

#define ROSA_LOGF_DEBUG(log, fmt, ...) (rosa__log_internal_logf(log, LOGL_DEBUG, fmt, ##__VA_ARGS__))
#define ROSA_LOGF_INFO(log, fmt, ...) (rosa__log_internal_logf(log, LOGL_INFO, fmt, ##__VA_ARGS__))
#define ROSA_LOGF_WARN(log, fmt, ...) (rosa__log_internal_logf(log, LOGL_WARN, fmt, ##__VA_ARGS__))
#define ROSA_LOGF_ERROR(log, fmt, ...) (rosa__log_internal_logf(log, LOGL_ERROR, fmt, ##__VA_ARGS__))
#define ROSA_LOGF_FATAL(log, fmt, ...) (rosa__log_internal_logf(log, LOGL_FATAL, fmt, ##__VA_ARGS__))

#define ROSA_LOGFV_DEBUG(log, fmt, args) (rosa__log_internal_logfv(log, LOGL_DEBUG, fmt, args))
#define ROSA_LOGFV_INFO(log, fmt, args) (rosa__log_internal_logfv(log, LOGL_INFO, fmt, args))
#define ROSA_LOGFV_WARN(log, fmt, args) (rosa__log_internal_logfv(log, LOGL_WARN, fmt, args))
#define ROSA_LOGFV_ERROR(log, fmt, args) (rosa__log_internal_logfv(log, LOGL_ERROR, fmt, args))
#define ROSA_LOGFV_FATAL(log, fmt, args) (rosa__log_internal_logfv(log, LOGL_FATAL, fmt, args))

#ifdef __cplusplus
}
#endif
