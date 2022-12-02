#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO strutil strcpy managed-strings

typedef struct {
    size_t cap; // char count available for storage in this string; includes space for the zero terminator char
    size_t len; // length of the string currently stored //TODO put in union together with ptr? //TODO how to make sure this is always valid after external editing? or disallow it entirely

    union {
        char imm[24]; // short string optimization
        char* ptr;
    };
} rosa_mstr;

//TODO proper error code type

//TODO how to differentiate null and empty str??

rosa_mstr* rosa_mstr_create(rosa_mstr* mstr, const char* cstr);

void rosa_mstr_destroy(rosa_mstr* mstr);

// this is mutable, be careful not to overstep the cap-1 (cap includes the zero terminator)
char* rosa_mstr_cstr(rosa_mstr* mstr);

int rosa_mstr_catm(rosa_mstr* mstr, rosa_mstr* mstr_cat);

int rosa_mstr_cat(rosa_mstr* mstr, const char* str_cat, const char* str_cat_end);

//TODO clone, copy_from, substr?, reserve
//TODO clear? insert, erase, append?, compare, startswith, endswith, contains, replace, substr, copy, resize?, swap?

//TODO conversions to various data types

#ifdef __cplusplus
}
#endif
