#include <stdio.h>
#include <stdlib.h>

#include "rosalia/util.h"

#ifdef __cplusplus
extern "C" {
#endif

//TODO use log callback instead of fprintf

void panic_unreachable_reached(const char* file, int line, int counter)
{
    // log_cb(LOGS_FATAL, "unreachable code reached: %s:%i #%i\n", file, line, counter);
    fprintf(stderr, "unreachable code reached: %s:%i #%i\n", file, line, counter);
    exit(EXIT_FAILURE);
}

void panic_rtassert_failed(bool cond, const char* msg, const char* file, int line, int counter)
{
    if (cond) {
        return;
    }
    fprintf(stderr, "rt-assert failed: %s:%i #%i", file, line, counter);
    if (msg != NULL) {
        fprintf(stderr, " \"%s\"", msg);
    }
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

void panic_cassert_failed(bool cond, const char* msg, const char* file, int line, int counter)
{
    if (cond) {
        return;
    }
    fprintf(stderr, "debug-assert failed: %s:%i #%i\n", file, line, counter);
    if (msg != NULL) {
        fprintf(stderr, " \"%s\"", msg);
    }
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

#ifdef __cplusplus
}
#endif
