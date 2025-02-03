#include <stdio.h>
#include <stdlib.h>

#include "rosalia/util.h"

#ifdef __cplusplus
extern "C" {
#endif

//TODO use log callback instead of fprintf

void panic_unreachable_reached(const char* file, int line)
{
    // log_cb(LOGS_FATAL, "unreachable code reached: %s:%i\n", file, line);
    fprintf(stderr, "unreachable code reached: %s:%i\n", file, line);
    exit(1);
}

void panic_rtassert_failed(bool cond, const char* msg, const char* file, int line)
{
    if (cond) {
        return;
    }
    fprintf(stderr, "assert failed: %s:%i%s%s\n", file, line, msg != NULL ? " --: " : "", msg != NULL ? msg : "");
    exit(1);
}

#ifdef __cplusplus
}
#endif
