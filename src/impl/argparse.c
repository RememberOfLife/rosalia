#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "rosalia/argparse.h"

#ifdef __cplusplus
extern "C" {
#endif

argp_basic argp_basic_init(int argc, char** argv)
{
    return (argp_basic){
        .argc = argc,
        .argv = argv,
        .w_argc = argc - 1,
        .w_arg = NULL,
        .n_arg = NULL,
    };
}

bool argp_basic_process(argp_basic* argp)
{
    bool ret = (argp->w_argc > 0);
    argp->w_arg = argp->argv[argp->argc - (argp->w_argc--)];
    argp->n_arg = ((argp->w_argc > 0) ? argp->argv[argp->argc - argp->w_argc] : NULL);
    return ret;
}

// arity 0, returns true if detected
bool argp_basic_arg_a0(argp_basic* argp, const char* name)
{
    return (strcmp(argp->w_arg, name) == 0);
}

// arity 1, returns true if detected and set p_vstr to the supplied arg, p_vstr may result to NULL if there is no arg to use
bool argp_basic_arg_a1(argp_basic* argp, const char* name, const char** p_vstr)
{
    bool ret = (strcmp(argp->w_arg, name) == 0);
    argp->w_argc--;
    *p_vstr = argp->n_arg;
    return ret;
}

#ifdef __cplusplus
}
#endif
