#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define ROSALIA_LOG_IMPLEMENTATION
#include "rosalia/log.h"

#include "tests/tests.h"

void logger_output_ignore_push_line(rosa_logger_output* output, const char* str, const char* str_end)
{
    // pass
}

void logger_output_ignore_destroy(rosa_logger_output* output)
{
    // pass
}

rosa_logger_output logger_output_ignore = (rosa_logger_output){
    .push_line = logger_output_ignore_push_line,
    .destroy = logger_output_ignore_destroy,
};

void run_test_log()
{
    rosa_logger* log;
    assert(rosa_log_create(&log, "test logger", false, "%d{%Y-%m-%d_%H:%M:%S}.%f% %l% (%creation_ms%) [%p%]: ", "\n"));
    rosa_log_output_add(log, logger_output_ignore);

    ROSA_LOGF(log, LOGL_INFO, "1");
    ROSA_LOGF_ERROR(log, "abc");

    rosa_log_destroy(&log);

    printf(". log\n");
}
