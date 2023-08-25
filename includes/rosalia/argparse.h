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
#define ROSALIA_ARGPARSE_VERSION_PATCH 4

#ifdef __cplusplus
extern "C" {
#endif

/*TODO
formats:
. flag style ( -a --color -df -t 3 )
. value style ( in=15 out=./file.log ) like in `dd`
. verb style ( show thing list ) like in `ip`, this can support the top two styles interwoven

api styles ( both should be provided ):
. preregistered ( first register all accepted flags/values/verbpaths with defaults and help etc, this can auto generate help and a usage command, then provide args to argparse object and check the values of the preregistered things when required, like python argparse )
. switched create processing struct from args and then if else ask it one by one what arg it is, optimized by a strhash, then go to next arg and or extract param for arg, this can also auto generate gelp but wizh a lot more effort )
. autotyped ( like preregistered but nothing is preregistered, instead supplied args are assumed valid and stored, then just access flags/values via implicit api )

every value can be typed if wanted, str is the most generic and default type here

needs to perform arg quote and escape string escaping..

verb format is a more complicated interaction path..
*/

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
    if (ret == true) {
        argp->w_argc--;
        *p_vstr = argp->n_arg;
    }
    return ret;
}

#ifdef __cplusplus
}
#endif

#endif
