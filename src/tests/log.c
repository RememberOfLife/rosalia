#include <stdint.h>
#include <stdio.h>

#define ROSALIA_LOG_IMPLEMENTATION
#include "rosalia/log.h"

#include "tests/tests.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <rosalia/vector.h>

const char* logl_names[LOGL_COUNT] = {
    [LOGL_NONE] = "NONE",
    [LOGL_DEBUG] = "DEBUG",
    [LOGL_INFO] = "INFO",
    [LOGL_WARN] = "WARN",
    [LOGL_ERROR] = "ERROR",
    [LOGL_FATAL] = "FATAL",
};

void rosa_log_create(
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
size_t rosa__log_internal_layout(char* buf, size_t len, rosa_logger* log, const char* layout)
{
    //TODO
    if (buf == NULL) {
        return 100;
    }
    return sprintf(buf, "%s", layout);
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

    size_t len_pre = rosa__log_internal_layout(NULL, 0, log, log->pre_layout);
    size_t len_user = vsnprintf(NULL, 0, fmt, args);
    size_t len_post = rosa__log_internal_layout(NULL, 0, log, log->post_layout);

    char* pbuf = malloc(len_pre + len_user + len_post + 1);
    assert(pbuf);

    // vsnprintf(pubf, len)
    char* wbuf = pbuf;
    wbuf += rosa__log_internal_layout(wbuf, len_pre + 1, log, log->pre_layout);
    wbuf += vsnprintf(wbuf, len_user + 1, fmt, args);
    wbuf += rosa__log_internal_layout(wbuf, len_post + 1, log, log->post_layout);

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

void logger_output_stdout_push_line(rosa_logger_output* output, const char* str, const char* str_end)
{
    printf("%s", str);
}

void logger_output_stdout_destroy(rosa_logger_output* output)
{
    // pass
}

rosa_logger_output logger_output_stdout = (rosa_logger_output){
    .push_line = logger_output_stdout_push_line,
    .destroy = logger_output_stdout_destroy,
};

void run_test_log()
{
    rosa_logger* log;
    rosa_log_create(&log, "test logger", false, "%d{%Y-%m-%d %H:%M:%S}.%f %l [%p]: ", "\n");
    rosa_log_output_add(log, logger_output_stdout);

    ROSA_LOGF(log, LOGL_INFO, "1");
    ROSA_LOGF_ERROR(log, "abc");

    rosa_log_destroy(&log);

    printf(". log\n");
}
