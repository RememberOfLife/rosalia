#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "rosalia/arraylist.h"
#include "rosalia/noise.h"
#include "rosalia/util.h"

#include "rosalia/argparse.h"

#ifdef __cplusplus
extern "C" {
#endif

//TODO handle errors by putting them into the str arena and then it has to be destructed..

bool rosa_argpv_create(rosa_argpv* argp, int argc, char** argv)
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
        argp->entries[i].key_hash = rosa_strhash(argp->entries[i].key, NULL);
    }

    return true;
}

void rosa_argpv_destroy(rosa_argpv* argp)
{
    VEC_DESTROY(&argp->entries[0].key);
    VEC_DESTROY(&argp->entries);
}

bool rosa_argpv_exists(rosa_argpv* argp, const char* key)
{
    return rosa_argpv_find(argp, key) >= 0;
}

const char* rosa_argpv_val(rosa_argpv* argp, const char* key)
{
    int32_t idx = rosa_argpv_find(argp, key);
    if (idx < 0) {
        return NULL;
    }
    return argp->entries[idx].val;
}

const char* rosa_argpv_val_def(rosa_argpv* argp, const char* key, const char* def)
{
    const char* rval = rosa_argpv_val(argp, key);
    if (rval == NULL) {
        rval = def;
    }
    return rval;
}

bool rosa_argpv_val_eq(rosa_argpv* argp, const char* key, const char* val)
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

int32_t rosa_argpv_find(rosa_argpv* argp, const char* key)
{
    uint32_t key_hash = rosa_strhash(key, NULL);
    for (uint32_t i = 0; i < VEC_LEN(&argp->entries); i++) {
        if (argp->entries[i].key_hash == key_hash && strcmp(argp->entries[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

rosa_argpv_entry* rosa_argpv_entry_at(rosa_argpv* argp, int32_t idx)
{
    if (idx >= 0 && idx < VEC_LEN(&argp->entries)) {
        return &(argp->entries[idx]);
    }
    return NULL;
}

int32_t rosa_argpv_entry_count(rosa_argpv* argp)
{
    return VEC_LEN(&argp->entries);
}

#ifdef __cplusplus
}
#endif
