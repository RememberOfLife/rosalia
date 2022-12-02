#pragma once

#include <stdbool.h>

// https://github.com/floooh/sokol/blob/master/sokol_args.h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct argp_basic_s {
    int argc;
    char** argv;
    int w_argc;
    char* w_arg;
    char* n_arg;
} argp_basic;

argp_basic argp_basic_init(int argc, char** argv);

bool argp_basic_process(argp_basic* argp);

// arity 0, returns true if detected
bool argp_basic_arg_a0(argp_basic* argp, const char* name);

// arity 1, returns true if detected and set p_vstr to the supplied arg, p_vstr may result to NULL if there is no arg to use
bool argp_basic_arg_a1(argp_basic* argp, const char* name, const char** p_vstr);

#ifdef __cplusplus
}
#endif
