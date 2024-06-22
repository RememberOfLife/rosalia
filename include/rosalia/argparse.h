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
#define ROSALIA_ARGPARSE_VERSION_MINOR 2
#define ROSALIA_ARGPARSE_VERSION_PATCH 0

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

    union {

        char* key;
        uint32_t key_off;
    };

    union {

        char* val;
        uint32_t val_off;
    };
} rosa_argpv_entry;

typedef struct rosa_argpv_s {
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
#include "rosalia/util.h"

#ifdef __cplusplus
extern "C" {
#endif

//TODO handle errors by putting them into the str arena and then it has to be destructed..

ROSALIA__ARGPARSE_DEF bool rosa_argpv_create(rosa_argpv* argp, int argc, char** argv)
{
    argp->entries = NULL;
    if (argc == 0) {
        return true;
    }
    VEC_CREATE(&argp->entries, 16);

    char* str_arena;
    VEC_CREATE(&str_arena, 256);

    typedef enum PARSE_STATE_E {
        PARSE_STATE_KEY = 0,
        PARSE_STATE_VALUE,
        PARSE_STATE_NEXT,
    } PARSE_STATE;

    const uint32_t NO_VAL = UINT32_MAX;
    uint32_t str_store_idx = 0;
    for (int i = 0; i < argc; i++) {
        rosa_argpv_entry entry;
        entry.key_off = str_store_idx;
        entry.val_off = NO_VAL;
        PARSE_STATE ps = PARSE_STATE_KEY;
        char* str_in = argv[i];
        while (ps != PARSE_STATE_NEXT) {
            char cc = *(str_in++);
            switch (cc) {
                case '\0': {
                    ps = PARSE_STATE_NEXT;
                } break;
                case '\\': {
                    cc = *(str_in++);
                    switch (cc) {
                        case 'n': {
                            cc = '\n';
                        } break;
                        case 'r': {
                            cc = '\r';
                        } break;
                        case 't': {
                            cc = '\t';
                        } break;
                        case '\\': {
                            cc = '\\';
                        } break;
                        default: {
                            //TODO FAIL: unclosed
                            cc = '\\'; // for now just assume this
                        } break;
                    }
                } break;
                case '=': {
                    ps = PARSE_STATE_VALUE;
                    entry.val_off = str_store_idx + 1;
                    cc = '\0';
                } break;
                default: {
                    //pass
                } break;
            }
            VEC_PUSH(&str_arena, cc);
            str_store_idx++;
        }
        VEC_PUSH(&argp->entries, entry);
    }

    for (int i = 0; i < VEC_LEN(&argp->entries); i++) {
        argp->entries[i].key = &str_arena[argp->entries[i].key_off];
        if (argp->entries[i].val_off == NO_VAL) {
            argp->entries[i].val = NULL;
        } else {
            argp->entries[i].val = &str_arena[argp->entries[i].val_off];
        }
        argp->entries[i].key_hash = strhash(argp->entries[i].key, NULL);
    }

    return true;
}

ROSALIA__ARGPARSE_DEC void rosa_argpv_destroy(rosa_argpv* argp)
{
    VEC_DESTROY(&argp->entries[0].key);
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
    if (idx < 0) {
        return false;
    }
    if (argp->entries[idx].val == NULL && val == NULL) {
        return true;
    }
    if (val == NULL && argp->entries[idx].val != NULL) {
        return false;
    }
    if (argp->entries[idx].val == NULL && val != NULL) {
        return false;
    }
    if (strcmp(argp->entries[idx].val, val) != 0) {
        return false;
    }
    return true;
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
