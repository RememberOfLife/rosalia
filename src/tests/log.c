#include <stdint.h>
#include <stdio.h>

#define ROSALIA_LOG_IMPLEMENTATION
#include "rosalia/log.h"

#include "tests/tests.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

const char* loga[LOGL_COUNT] = {
    [LOGL_NONE] = "NONE",
    [LOGL_DEBUG] = "DEBUG",
    [LOGL_INFO] = "INFO",
    [LOGL_WARN] = "WARN",
    [LOGL_ERROR] = "ERROR",
    [LOGL_FATAL] = "FATAL",
};

typedef struct rosa_logger_s {
    const char* log_name;
    const char* pre_layout;
    const char* post_layout;
    //TODO callback and child loggers?
} rosa_logger;

void rosa_log_create(rosa_logger** log, const char* log_name, const char* pre_layout, const char* post_layout);
void rosa_log_destroy(rosa_logger** log);
void rosa_log(rosa_logger* log, LOGL lvl, const char* str, const char* str_end);
void rosa_logf(rosa_logger* log, LOGL lvl, const char* fmt, ...);
void rosa_logfv(rosa_logger* log, LOGL lvl, const char* fmt, va_list args);

void rosa_log_create(rosa_logger** log, const char* log_name, const char* pre_layout, const char* post_layout)
{
    *log = malloc(sizeof(rosa_logger));
    rosa_logger* logi = *log;
    logi->log_name = strdup(log_name);
    logi->pre_layout = strdup(pre_layout);
    logi->post_layout = strdup(post_layout);
}

void rosa_log_destroy(rosa_logger** log)
{
    rosa_logger* logi = *log;
    free(logi->log_name);
    free(logi->pre_layout);
    free(logi->post_layout);
    free(logi);
    *log = NULL;
}

// behaves similar to how snprintf would
size_t rosa__log_internal_layout(char* buf, size_t len, rosa_logger* log, const char* layout)
{
    //TODO
}

void rosa_log(rosa_logger* log, LOGL lvl, const char* str, const char* str_end)
{
    if (str_end == NULL) {
        return rosa_logf(log, lvl, "%s", str);
    } else {
        return rosa_logf(log, lvl, "%.*s", str_end - str, str);
    }
}

void rosa_logf(rosa_logger* log, LOGL lvl, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    rosa_logfv(log, lvl, fmt, args);
    va_end(args);
}

void rosa_logfv(rosa_logger* log, LOGL lvl, const char* fmt, va_list args)
{
    assert(log != NULL);
    assert(lvl > LOGL_NONE && lvl < LOGL_COUNT);
    assert(fmt);

    size_t len_pre = rosa__log_internal_layout(NULL, 0, log, log->pre_layout);
    size_t len_user = vsnprintf(NULL, 0, fmt, args);
    size_t len_post = rosa__log_internal_layout(NULL, 0, log, log->post_layout);

    char* pbuf = malloc(len_pre + len_user + len_post + 1);
    assert(pbuf);

    // vsnprintf(pubf, len)
    char* wbuf = pbuf;
    wbuf += rosa__log_internal_layout(wbuf, len_pre + 1, log, log->pre_layout);
    wbuf += vsnprintf(wbuf, len_user + 1, fmt, args);
    wbuf += rosa__log_internal_layout(wbuf, len_post, log, log->post_layout);

    // push to callbacks etc..
    printf("%s", pbuf);
    free(pbuf);
}

//TODO these all should pass their file and line number to the log function

#define ROSA_LOG_DEBUG(log, str, str_end) (rosa_log(log, LOGL_DEBUG, str, str_end))
#define ROSA_LOG_INFO(log, str, str_end) (rosa_log(log, LOGL_INFO, str, str_end))
#define ROSA_LOG_WARN(log, str, str_end) (rosa_log(log, LOGL_WARN, str, str_end))
#define ROSA_LOG_ERROR(log, str, str_end) (rosa_log(log, LOGL_ERROR, str, str_end))
#define ROSA_LOG_FATAL(log, str, str_end) (rosa_log(log, LOGL_FATAL, str, str_end))

#define ROSA_LOGF_DEBUG(log, fmt, ...) (rosa_logf(log, LOGL_DEBUG, fmt, ##__VA_ARGS__))
#define ROSA_LOGF_INFO(log, fmt, ...) (rosa_logf(log, LOGL_INFO, fmt, ##__VA_ARGS__))
#define ROSA_LOGF_WARN(log, fmt, ...) (rosa_logf(log, LOGL_WARN, fmt, ##__VA_ARGS__))
#define ROSA_LOGF_ERROR(log, fmt, ...) (rosa_logf(log, LOGL_ERROR, fmt, ##__VA_ARGS__))
#define ROSA_LOGF_FATAL(log, fmt, ...) (rosa_logf(log, LOGL_FATAL, fmt, ##__VA_ARGS__))

#define ROSA_LOGFV_DEBUG(log, fmt, args) (rosa_logf(log, LOGL_DEBUG, fmt, args))
#define ROSA_LOGFV_INFO(log, fmt, args) (rosa_logf(log, LOGL_INFO, fmt, args))
#define ROSA_LOGFV_WARN(log, fmt, args) (rosa_logf(log, LOGL_WARN, fmt, args))
#define ROSA_LOGFV_ERROR(log, fmt, args) (rosa_logf(log, LOGL_ERROR, fmt, args))
#define ROSA_LOGFV_FATAL(log, fmt, args) (rosa_logf(log, LOGL_FATAL, fmt, args))

void run_test_log()
{
    rosa_logger* log;
    rosa_log_create(&log, "test logger", "%d{%Y-%m-%d_%H:%M:%S} %l [%p]: ", "\n");

    rosa_logf(log, LOGL_INFO, "1");
    ROSA_LOGF_ERROR(log, "abc");

    rosa_log_destroy(&log);

    printf(". log\n");
}
