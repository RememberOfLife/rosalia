#ifndef ROSALIA_ARGPARSE_H_INCLUDE
#define ROSALIA_ARGPARSE_H_INCLUDE

#include <stdbool.h>

#ifdef ROSALIA_ARGPARSE_STATIC
#define ROSALIA__ARGPARSE_DEC static
#define ROSALIA__ARGPARSE_DEC_EXT static
#define ROSALIA__ARGPARSE_DEF static
#else
#define ROSALIA__ARGPARSE_DEC
#define ROSALIA__ARGPARSE_DEC_EXT extern
#define ROSALIA__ARGPARSE_DEF
#endif

#ifdef ROSALIA_ARGPARSE_DECORATE
#define ROSALIA__ARGPARSE_DECORATE(ident) ROSALIA_ARGPARSE_DECORATE(ident)
#else
#define ROSALIA__ARGPARSE_DECORATE(ident) ident
#endif

#define ROSALIA_ARGPARSE_VERSION_MAJOR 0
#define ROSALIA_ARGPARSE_VERSION_MINOR 1
#define ROSALIA_ARGPARSE_VERSION_PATCH 3

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

ROSALIA__ARGPARSE_DEC argp_basic ROSALIA__ARGPARSE_DECORATE(argp_basic_init)(int argc, char** argv);

ROSALIA__ARGPARSE_DEC bool ROSALIA__ARGPARSE_DECORATE(argp_basic_process)(argp_basic* argp);

// arity 0, returns true if detected
ROSALIA__ARGPARSE_DEC bool ROSALIA__ARGPARSE_DECORATE(argp_basic_arg_a0)(argp_basic* argp, const char* name);

// arity 1, returns true if detected and set p_vstr to the supplied arg, p_vstr may result to NULL if there is no arg to use
ROSALIA__ARGPARSE_DEC bool ROSALIA__ARGPARSE_DECORATE(argp_basic_arg_a1)(argp_basic* argp, const char* name, const char** p_vstr);

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_ARGPARSE_IMPLEMENTATION) && !defined(ROSALIA_ARGPARSE_H_IMPL)
#define ROSALIA_ARGPARSE_H_IMPL

#define ROSALIA__ARGPARSE_INTERNAL(ident) rosalia__argparse_internal_##ident

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__ARGPARSE_DEF argp_basic ROSALIA__ARGPARSE_DECORATE(argp_basic_init)(int argc, char** argv)
{
    return (argp_basic){
        .argc = argc,
        .argv = argv,
        .w_argc = argc - 1,
        .w_arg = NULL,
        .n_arg = NULL,
    };
}

ROSALIA__ARGPARSE_DEF bool ROSALIA__ARGPARSE_DECORATE(argp_basic_process)(argp_basic* argp)
{
    bool ret = (argp->w_argc > 0);
    argp->w_arg = argp->argv[argp->argc - (argp->w_argc--)];
    argp->n_arg = ((argp->w_argc > 0) ? argp->argv[argp->argc - argp->w_argc] : NULL);
    return ret;
}

// arity 0, returns true if detected
ROSALIA__ARGPARSE_DEF bool ROSALIA__ARGPARSE_DECORATE(argp_basic_arg_a0)(argp_basic* argp, const char* name)
{
    return (strcmp(argp->w_arg, name) == 0);
}

// arity 1, returns true if detected and set p_vstr to the supplied arg, p_vstr may result to NULL if there is no arg to use
ROSALIA__ARGPARSE_DEF bool ROSALIA__ARGPARSE_DECORATE(argp_basic_arg_a1)(argp_basic* argp, const char* name, const char** p_vstr)
{
    bool ret = (strcmp(argp->w_arg, name) == 0);
    argp->w_argc--;
    *p_vstr = argp->n_arg;
    return ret;
}

#ifdef __cplusplus
}
#endif

#endif
