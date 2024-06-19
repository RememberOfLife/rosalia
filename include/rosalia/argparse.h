#ifndef ROSALIA_ARGPARSE_H_INCLUDE
#define ROSALIA_ARGPARSE_H_INCLUDE

#include <stdbool.h>
#include <stdint.h>

#ifdef ROSALIA_ARGPARSE_STATIC
#define ROSALIA__ARGPARSE_DEC static
#define ROSALIA__ARGPARSE_DEC_EXT static
#define ROSALIA__ARGPARSE_DEF static
#else
#define ROSALIA__ARGPARSE_DEC
#define ROSALIA__ARGPARSE_DEC_EXT extern
#define ROSALIA__ARGPARSE_DEF
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

typedef struct rosa_argpv_entry_s {
    uint32_t key_hash;
    const char* key;
    const char* val;
} rosa_argpv_entry;

typedef struct rosa_argpv_s {
    //TODO do we want to modify the input argv or store our own str arena?
    int argc;
    char** argv;
    rosa_argpv_entry* entries; // rosa vec
} rosa_argpv;

//TODO what behaviour do we want on multiple same named keys? should be illegal or offer all of them

// returns true if successful
ROSALIA__ARGPARSE_DEC bool rosa_argpv_create(rosa_argpv* argp, int argc, char** argv);

ROSALIA__ARGPARSE_DEC void rosa_argpv_destroy(rosa_argpv* argp);

// returns true if an entry with the key exists
ROSALIA__ARGPARSE_DEC bool rosa_argpv_exists(rosa_argpv* argp, const char* key);

ROSALIA__ARGPARSE_DEC const char* rosa_argpv_val(rosa_argpv* argp, const char* key);

ROSALIA__ARGPARSE_DEC const char* rosa_argpv_val_def(rosa_argpv* argp, const char* key, const char* def);

ROSALIA__ARGPARSE_DEC bool rosa_argpv_val_eq(rosa_argpv* argp, const char* key, const char* val);

ROSALIA__ARGPARSE_DEC int32_t rosa_argpv_find(rosa_argpv* argp, const char* key);

ROSALIA__ARGPARSE_DEC rosa_argpv_entry* rosa_argpv_entry_at(rosa_argpv* argp, int32_t idx);

ROSALIA__ARGPARSE_DEC int32_t rosa_argpv_entry_count(rosa_argpv* argp);

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_ARGPARSE_IMPLEMENTATION) && !defined(ROSALIA_ARGPARSE_H_IMPL)
#define ROSALIA_ARGPARSE_H_IMPL

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "rosalia/noise.h"
#include "rosalia/vector.h"

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__ARGPARSE_DEF bool rosa_argpv_create(rosa_argpv* argp, int argc, char** argv)
{
    *argp = (rosa_argpv){
        .argc = argc,
        .argv = argv,
    };
    VEC_CREATE(&argp->entries, 16);

    //TODO for now modify argv, later str arena it
    for (int i = 0; i < argp->argc; i++) {
        char* warg = argv[i];
        //TODO search until first =
        //TODO create key cutoff in argv and key str hash
        //TODO parse rest as value, respecting quotes, i.e. might go on to next argument
    }

    return true;
}

ROSALIA__ARGPARSE_DEC void rosa_argpv_destroy(rosa_argpv* argp)
{
    VEC_DESTROY(&argp->entries);
}

ROSALIA__ARGPARSE_DEC bool rosa_argpv_exists(rosa_argpv* argp, const char* key)
{
    return rosa_argpv_find(argp, key) >= 0;
}

ROSALIA__ARGPARSE_DEC const char* rosa_argpv_val(rosa_argpv* argp, const char* key)
{
    int32_t idx = rosa_argpv_find(argp, key);
    if (idx < 0) {
        return NULL;
    }
    return argp->entries[idx].val;
}

ROSALIA__ARGPARSE_DEC const char* rosa_argpv_val_def(rosa_argpv* argp, const char* key, const char* def)
{
    const char* rval = rosa_argpv_val(argp, key);
    if (rval == NULL) {
        rval = def;
    }
    return rval;
}

ROSALIA__ARGPARSE_DEC bool rosa_argpv_val_eq(rosa_argpv* argp, const char* key, const char* val)
{
    int32_t idx = rosa_argpv_find(argp, key);
    return idx >= 0 && strcmp(argp->entries[idx].val, val) == 0;
}

ROSALIA__ARGPARSE_DEC int32_t rosa_argpv_find(rosa_argpv* argp, const char* key)
{
    uint32_t key_hash = strhash(key, NULL);
    for (uint32_t i = 0; i < VEC_LEN(&argp->entries); i++) {
        if (argp->entries[i].key_hash == key_hash && strcmp(argp->entries[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

ROSALIA__ARGPARSE_DEC rosa_argpv_entry* rosa_argpv_entry_at(rosa_argpv* argp, int32_t idx)
{
    if (idx >= 0 && idx < VEC_LEN(&argp->entries)) {
        return &(argp->entries[idx]);
    }
    return NULL;
}

ROSALIA__ARGPARSE_DEC int32_t rosa_argpv_entry_count(rosa_argpv* argp)
{
    return VEC_LEN(&argp->entries);
}

#ifdef __cplusplus
}
#endif

#endif
