#ifndef ROSALIA_ARGPARSE_H_INCLUDE
#define ROSALIA_ARGPARSE_H_INCLUDE

#include <stdbool.h>

#ifdef ROSALIA_ARGPARSE_STATIC
#define ROSALIA__DEC static
#define ROSALIA__DEC_EXT static
#define ROSALIA__DEF static
#else
#define ROSALIA__DEC
#define ROSALIA__DEC_EXT extern
#define ROSALIA__DEF
#endif

#ifdef ROSALIA_ARGPARSE_DECORATE
#define ROSALIA__DECORATE(ident) ROSALIA_ARGPARSE_DECORATE(ident)
#else
#define ROSALIA__DECORATE(ident) ident
#endif

#define ROSALIA_ARGPARSE_VERSION_MAJOR 0
#define ROSALIA_ARGPARSE_VERSION_MINOR 1
#define ROSALIA_ARGPARSE_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

// https://github.com/floooh/sokol/blob/master/sokol_args.h

//TODO decorate struct
typedef struct argp_basic_s {
    int argc;
    char** argv;
    int w_argc;
    char* w_arg;
    char* n_arg;
} argp_basic;

ROSALIA__DEC argp_basic ROSALIA__DECORATE(argp_basic_init)(int argc, char** argv);

ROSALIA__DEC bool ROSALIA__DECORATE(argp_basic_process)(argp_basic* argp);

// arity 0, returns true if detected
ROSALIA__DEC bool ROSALIA__DECORATE(argp_basic_arg_a0)(argp_basic* argp, const char* name);

// arity 1, returns true if detected and set p_vstr to the supplied arg, p_vstr may result to NULL if there is no arg to use
ROSALIA__DEC bool ROSALIA__DECORATE(argp_basic_arg_a1)(argp_basic* argp, const char* name, const char** p_vstr);

#ifdef __cplusplus
}
#endif

#endif

#ifdef ROSALIA_ARGPARSE_IMPLEMENTATION

#define ROSALIA__INTERNAL(ident) rosalia__internal_##ident

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__DEF argp_basic ROSALIA__DECORATE(argp_basic_init)(int argc, char** argv)
{
    return (argp_basic){
        .argc = argc,
        .argv = argv,
        .w_argc = argc - 1,
        .w_arg = NULL,
        .n_arg = NULL,
    };
}

ROSALIA__DEF bool ROSALIA__DECORATE(argp_basic_process)(argp_basic* argp)
{
    bool ret = (argp->w_argc > 0);
    argp->w_arg = argp->argv[argp->argc - (argp->w_argc--)];
    argp->n_arg = ((argp->w_argc > 0) ? argp->argv[argp->argc - argp->w_argc] : NULL);
    return ret;
}

// arity 0, returns true if detected
ROSALIA__DEF bool ROSALIA__DECORATE(argp_basic_arg_a0)(argp_basic* argp, const char* name)
{
    return (strcmp(argp->w_arg, name) == 0);
}

// arity 1, returns true if detected and set p_vstr to the supplied arg, p_vstr may result to NULL if there is no arg to use
ROSALIA__DEF bool ROSALIA__DECORATE(argp_basic_arg_a1)(argp_basic* argp, const char* name, const char** p_vstr)
{
    bool ret = (strcmp(argp->w_arg, name) == 0);
    argp->w_argc--;
    *p_vstr = argp->n_arg;
    return ret;
}

#ifdef __cplusplus
}
#endif

#undef ROSALIA__INTERNAL
#endif

#undef ROSALIA__DEC
#undef ROSALIA__DEC_EXT
#undef ROSALIA__DEF
#undef ROSALIA__DECORATE
