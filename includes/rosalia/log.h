#ifndef ROSALIA_LOG_H_INCLUDE
#define ROSALIA_LOG_H_INCLUDE

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_LOG_STATIC
#define ROSALIA__LOG_DEC static
#define ROSALIA__LOG_DEC_EXT static
#define ROSALIA__LOG_DEF static
#else
#define ROSALIA__LOG_DEC
#define ROSALIA__LOG_DEC_EXT extern
#define ROSALIA__LOG_DEF
#endif

#ifdef ROSALIA_LOG_DECORATE
#define ROSALIA__LOG_DECORATE(ident) ROSALIA_LOG_DECORATE(ident)
#else
#define ROSALIA__LOG_DECORATE(ident) ident
#endif

#define ROSALIA_LOG_VERSION_MAJOR 0
#define ROSALIA_LOG_VERSION_MINOR 1
#define ROSALIA_LOG_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

/*TODO api? whats the intended usage/pattern here?
- backends for writing to files, printing, in-memory cycle buffer, //TODO composable??
- debug log statements that compile out of release builds, but can be on demand disabled
*/

//NOTE: this depends on timestamp.h, as "rosalia/timestamp.h"
//NOTE: this depends on vector.h, as "rosalia/vector.h"

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
    uint64_t creation_ms;
    char* log_name;
    char* pre_layout;
    char* post_layout;
    bool concurrent_usage;
    //TODO child loggers?
    uint32_t next_output_id;
    rosa_logger_output* outputs; // rosa_vec<rosa_logger_output>
} rosa_logger;

// true on success, false on fail and auto destructs
ROSALIA__LOG_DEC bool rosa_log_create(rosa_logger** log, const char* log_name, bool concurrent_usage, const char* pre_layout, const char* post_layout);

ROSALIA__LOG_DEC void rosa_log_destroy(rosa_logger** log);

ROSALIA__LOG_DEC uint32_t rosa_log_output_add(rosa_logger* log, rosa_logger_output output);

ROSALIA__LOG_DEC bool rosa_log_output_remove(rosa_logger* log, uint32_t output_id, rosa_logger_output* output);

static const size_t ROSA__LOG_INTERNAL_LAYOUT_ERR = SIZE_MAX;
ROSALIA__LOG_DEC size_t rosa__log_internal_layout(char* buf, size_t len, rosa_logger* log, LOGL lvl, const char* layout);

ROSALIA__LOG_DEC void rosa__log_internal_log(rosa_logger* log, LOGL lvl, const char* str, const char* str_end);

ROSALIA__LOG_DEC void rosa__log_internal_logf(rosa_logger* log, LOGL lvl, const char* fmt, ...);

ROSALIA__LOG_DEC void rosa__log_internal_logfv(rosa_logger* log, LOGL lvl, const char* fmt, va_list args);

//TODO these all should pass their file and line number to the log function and make it available as a conversion specifier in the layout

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

#endif

#if defined(ROSALIA_LOG_IMPLEMENTATION) && !defined(ROSALIA_LOG_H_IMPL)
#define ROSALIA_LOG_H_IMPL

#define ROSALIA__LOG_INTERNAL(ident) rosalia__log_internal_##ident

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rosalia/timestamp.h"
#include "rosalia/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

const char* logl_names[LOGL_COUNT] = {
    [LOGL_NONE] = "NONE",
    [LOGL_DEBUG] = "DEBUG",
    [LOGL_INFO] = "INFO",
    [LOGL_WARN] = "WARN",
    [LOGL_ERROR] = "ERROR",
    [LOGL_FATAL] = "FATAL",
};

bool rosa_log_create(
    rosa_logger** log,
    const char* log_name,
    bool concurrent_usage, //TODO this / using the mutexes should be guarded by a define, i.e. make it possible to use logging without providing pthreads
    const char* pre_layout,
    const char* post_layout
)
{
    *log = malloc(sizeof(rosa_logger));
    rosa_logger* logi = *log;
    logi->log_name = strdup(log_name);
    logi->pre_layout = strdup(pre_layout);
    logi->post_layout = strdup(post_layout);
    logi->concurrent_usage = concurrent_usage;
    logi->next_output_id = 0;
    VEC_CREATE(&logi->outputs, 1);

    if (
        rosa__log_internal_layout(NULL, 0, logi, LOGL_NONE, logi->pre_layout) == ROSA__LOG_INTERNAL_LAYOUT_ERR ||
        rosa__log_internal_layout(NULL, 0, logi, LOGL_NONE, logi->post_layout) == ROSA__LOG_INTERNAL_LAYOUT_ERR
    ) {
        rosa_log_destroy(log);
        return false;
    }
    logi->creation_ms = timestamp_get_ms64();
    return true;
}

void rosa_log_destroy(rosa_logger** log)
{
    rosa_logger* logi = *log;
    if (logi->concurrent_usage == true) {
        //TODO locking..
    }
    free(logi->log_name);
    free(logi->pre_layout);
    free(logi->post_layout);
    free(logi);
    for (size_t i = 0; i < VEC_LEN(&logi->outputs); i++) {
        logi->outputs[i].destroy(&logi->outputs[i]);
    }
    VEC_DESTROY(&logi->outputs);
    *log = NULL;
}

// ownership of the private data in output is given to the logger and will be destroyed through the callback later
uint32_t rosa_log_output_add(rosa_logger* log, rosa_logger_output output)
{
    if (log->concurrent_usage == true) {
        //TODO locking..
    }
    output.id = log->next_output_id++;
    VEC_PUSH(&log->outputs, output);
    if (log->concurrent_usage == true) {
        //TODO locking..
    }
    return output.id;
}

// must use an id previously returned through output_add and not yet removed
// returns true if the removal took place
// NOTE: a removed logger is NOT destroyed, it is moved to output and the ownership of everything therein with it
bool rosa_log_output_remove(rosa_logger* log, uint32_t output_id, rosa_logger_output* output)
{
    if (log->concurrent_usage == true) {
        //TODO locking..
    }
    for (size_t i = 0; i < VEC_LEN(&log->outputs); i++) {
        if (log->outputs[i].id == output_id) {
            *output = log->outputs[i];
            VEC_REMOVE_SWAP(&log->outputs, i);
            if (log->concurrent_usage == true) {
                //TODO locking..
            }
            return true;
        }
    }
    if (log->concurrent_usage == true) {
        //TODO locking..
    }
    return false;
}

// behaves similar to how snprintf would
size_t rosa__log_internal_layout(char* buf, size_t len, rosa_logger* log, LOGL lvl, const char* layout)
{
    uint64_t log_ms = timestamp_get_ms64();
    timestamp log_timestamp = timestamp_get();
    time_t log_rawtime = log_timestamp.time;
    struct tm log_timeinfo;
    localtime_r(&log_rawtime, &log_timeinfo);

    //TODO maybe it would be better if the layouts are user supplied callbacks? then everybody can bring their own special things

    size_t acc_len = 0;
    char* wbuf = buf;
    const char* cstr = layout;

    typedef enum PARSING_STATE_E {
        PARSING_STATE_ANY = 0,
        PARSING_STATE_CONVERSION_SPECIFIER,
        PARSING_STATE_CONVERSION_DEFINITION,
        PARSING_STATE_CONVERSION_NOW,
        PARSING_STATE_DONE,
    } PARSING_STATE;

    PARSING_STATE pstate = PARSING_STATE_ANY;

    //HACK static buf sizes
    char conversion_specifier[32] = "\0";
    size_t conversion_specifier_len = 0;
    char conversion_definition[256] = "\0";
    size_t conversion_definition_len = 0;

    while (pstate != PARSING_STATE_DONE) {
        switch (pstate) {
            case PARSING_STATE_ANY: {
                switch (*cstr) {
                    case '%': {
                        pstate = PARSING_STATE_CONVERSION_SPECIFIER;
                    } break;
                    case '\0': {
                        pstate = PARSING_STATE_DONE;
                    } break;
                    default: {
                        if (buf != NULL) {
                            *wbuf = *cstr;
                            wbuf++;
                        }
                        acc_len++;
                    } break;
                }
                cstr++;
            } break;
            case PARSING_STATE_CONVERSION_SPECIFIER: {
                switch (*cstr) {
                    case '%': {
                        if (conversion_specifier_len == 0) {
                            // this is the escape for actually printing percentage signs
                            if (wbuf != NULL) {
                                *wbuf = *cstr;
                                wbuf++;
                            }
                            acc_len++;
                            pstate = PARSING_STATE_ANY;
                        } else {
                            pstate = PARSING_STATE_CONVERSION_NOW;
                        }
                    } break;
                    case '{': {
                        if (conversion_specifier_len == 0) {
                            return ROSA__LOG_INTERNAL_LAYOUT_ERR;
                        } else {
                            pstate = PARSING_STATE_CONVERSION_DEFINITION;
                        }
                    } break;
                    case '\0': {
                        return ROSA__LOG_INTERNAL_LAYOUT_ERR;
                    } break;
                    default: {
                        if (conversion_specifier_len >= sizeof(conversion_specifier)) {
                            return ROSA__LOG_INTERNAL_LAYOUT_ERR;
                        }
                        conversion_specifier[conversion_specifier_len] = *cstr;
                        conversion_specifier_len++;
                        conversion_specifier[conversion_specifier_len] = '\0';
                    } break;
                }
                cstr++;
            } break;
            case PARSING_STATE_CONVERSION_DEFINITION: {
                switch (*cstr) {
                    case '}': {
                        pstate = PARSING_STATE_CONVERSION_NOW;
                    } break;
                    case '\0': {
                        return ROSA__LOG_INTERNAL_LAYOUT_ERR;
                    } break;
                    default: {
                        if (conversion_definition_len >= sizeof(conversion_definition)) {
                            return ROSA__LOG_INTERNAL_LAYOUT_ERR;
                        }
                        conversion_definition[conversion_definition_len] = *cstr;
                        conversion_definition_len++;
                        conversion_definition[conversion_definition_len] = '\0';
                    } break;
                }
                cstr++;
            } break;
            case PARSING_STATE_CONVERSION_NOW: {
                // fully process specifier with definition
                assert(conversion_specifier_len > 0);

                char* l_buf = (wbuf == NULL ? NULL : wbuf);
                size_t l_len = (wbuf == NULL ? 0 : len - acc_len);
                size_t a_len;

                if (strcmp(conversion_specifier, "d") == 0) {
                    if (l_buf == NULL) {
                        //HACK these may be too small
                        char time_str_buf[256];
                        a_len = strftime(time_str_buf, sizeof(time_str_buf), conversion_definition, &log_timeinfo);
                    } else {
                        a_len = strftime(l_buf, l_len, conversion_definition, &log_timeinfo);
                    }
                } else if (strcmp(conversion_specifier, "f") == 0) {
                    a_len = snprintf(l_buf, l_len, "%03u", log_timestamp.fraction / 1000000);
                } else if (strcmp(conversion_specifier, "creation_ms") == 0) {
                    a_len = snprintf(l_buf, l_len, "%lu", log_ms - log->creation_ms);
                } else if (strcmp(conversion_specifier, "l") == 0) {
                    a_len = snprintf(l_buf, l_len, "%s", log->log_name);
                } else if (strcmp(conversion_specifier, "p") == 0) {
                    if (lvl >= LOGL_COUNT) {
                        lvl = LOGL_NONE;
                    }
                    a_len = snprintf(l_buf, l_len, "%s", logl_names[lvl]);
                } else {
                    return ROSA__LOG_INTERNAL_LAYOUT_ERR;
                }

                acc_len += a_len;
                if (wbuf != NULL) {
                    wbuf += a_len;
                }

                conversion_specifier_len = 0;
                conversion_specifier[0] = '\0';
                conversion_definition_len = 0;
                conversion_definition[0] = '\0';
                pstate = PARSING_STATE_ANY;
            } break;
            default: {
                assert(0);
            } break;
        }
    }

    if (wbuf != NULL) {
        wbuf[acc_len] = '\0';
    }

    return acc_len;
}

void rosa__log_internal_log(rosa_logger* log, LOGL lvl, const char* str, const char* str_end)
{
    if (str_end == NULL) {
        return rosa__log_internal_logf(log, lvl, "%s", str);
    } else {
        return rosa__log_internal_logf(log, lvl, "%.*s", str_end - str, str);
    }
}

void rosa__log_internal_logf(rosa_logger* log, LOGL lvl, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    rosa__log_internal_logfv(log, lvl, fmt, args);
    va_end(args);
}

void rosa__log_internal_logfv(rosa_logger* log, LOGL lvl, const char* fmt, va_list args)
{
    assert(log != NULL);
    assert(lvl > LOGL_NONE && lvl < LOGL_COUNT);
    assert(fmt);

    size_t len_pre = rosa__log_internal_layout(NULL, 0, log, lvl, log->pre_layout);
    size_t len_user = vsnprintf(NULL, 0, fmt, args);
    size_t len_post = rosa__log_internal_layout(NULL, 0, log, lvl, log->post_layout);

    char* pbuf = malloc(len_pre + len_user + len_post + 1);
    assert(pbuf);

    char* wbuf = pbuf;
    wbuf += rosa__log_internal_layout(wbuf, len_pre + 1, log, lvl, log->pre_layout);
    wbuf += vsnprintf(wbuf, len_user + 1, fmt, args);
    wbuf += rosa__log_internal_layout(wbuf, len_post + 1, log, lvl, log->post_layout);

    if (log->concurrent_usage == true) {
        //TODO locking..
    }
    // push to callbacks etc..
    for (size_t i = 0; i < VEC_LEN(&log->outputs); i++) {
        log->outputs[i].push_line(&log->outputs[i], pbuf, wbuf + 1);
    }
    if (log->concurrent_usage == true) {
        //TODO locking..
    }

    free(pbuf);
}

#ifdef __cplusplus
}
#endif

#endif
