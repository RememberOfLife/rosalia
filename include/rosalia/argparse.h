#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "rosalia/arraylist.h"
#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

static const rosa_semver rosalia_argparse_version = (rosa_semver){0, 3, 0};

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
    ROSA_ARRAYLIST(rosa_argpv_entry) entries;
} rosa_argpv;

//TODO what behaviour do we want on multiple same named keys? should be illegal or offer all of them

// returns true if successful
bool rosa_argpv_create(rosa_argpv* argp, int argc, char** argv);

void rosa_argpv_destroy(rosa_argpv* argp);

// returns true if an entry with the key exists
bool rosa_argpv_exists(rosa_argpv* argp, const char* key);

const char* rosa_argpv_val(rosa_argpv* argp, const char* key);

const char* rosa_argpv_val_def(rosa_argpv* argp, const char* key, const char* def);

bool rosa_argpv_val_eq(rosa_argpv* argp, const char* key, const char* val);

int32_t rosa_argpv_find(rosa_argpv* argp, const char* key);

rosa_argpv_entry* rosa_argpv_entry_at(rosa_argpv* argp, int32_t idx);

int32_t rosa_argpv_entry_count(rosa_argpv* argp);

#ifdef __cplusplus
}
#endif
