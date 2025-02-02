#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
https://www.w3schools.com/c/c_ref_string.php
https://github.com/mickjc750/str
https://github.com/antirez/sds string is packed with the meta information: len, cap, all the str bytes, zero; instead of a struct
*/

//TODO views? and or basic string as mutable+extendable view? maybe with parent pointer which can grow, but invalidates other string views..? or maybe all created views are known to the parent

static const size_t MSTR_MAX = SIZE_MAX;

typedef struct {
    bool internal : 1;
    bool read_only : 1;
    bool view : 1;
    bool _r4 : 1; //TODO created flag? also maybe use a canary in the 7 padding bytes, for allocated strings, which can fail much more quickly..
    bool _r5 : 1;
    bool _r6 : 1;
    bool _r7 : 1;
    bool _r8 : 1;
} _rosa_str_flags;

typedef struct {
    char buf[31];
    _rosa_str_flags flags;
} _rosa_istr;

typedef struct {
    size_t cap; // available storage bytes in this string; includes space for the zero terminator char
    size_t len; // byte length of the string currently stored
    char* ptr;
    uint8_t _padding[7];
    _rosa_str_flags flags;
} _rosa_astr;

// length changing external editing is not allowed
typedef struct {
    union {
        _rosa_istr imm; // short string optimization
        _rosa_astr ptr;
    };
} rosa_mstr;

//TODO proper error code type

//TODO how to differentiate =null and ="" str??

//TODO for creation, auto allocate, or someone else does it for us?

// if non NULL, the provided target will be where the mstr is created into
// if target is NULL, a new mstr will be allocated
// returns: ptr to the mstr object
rosa_mstr* rosa_mstr_create_with_cap(rosa_mstr* dst, size_t min_cap);
// same as rosa_mstr_create_with_cap
rosa_mstr* rosa_mstr_create_from_cstr(rosa_mstr* dst, const char* cstr);

void rosa_mstr_destroy(rosa_mstr* dst);

size_t rosa_mstr_len(rosa_mstr* src);

// this is mutable, be careful not to overstep the cap-1 (cap includes the zero terminator)
char* rosa_mstr_cstr(rosa_mstr* src);

void rosa_mstr_catm(rosa_mstr* dst, rosa_mstr* src);

void rosa_mstr_cat(rosa_mstr* dst, const char* src, const char* src_end);

// dst is not created, and will be created
rosa_mstr* rosa_mstr_clone(rosa_mstr* dst, rosa_mstr* src);

// dst is a created mstr, and will be overwritten completely
void rosa_mstr_copy_from(rosa_mstr* dst, rosa_mstr* src);

void rosa_mstr_clear(rosa_mstr* dst);

bool rosa_mstr_empty(rosa_mstr* src);

rosa_mstr* rosa_mstr_substr(rosa_mstr* dst, rosa_mstr* src, size_t len);

void rosa_mstr_reserve_cap(rosa_mstr* dst, size_t min_cap);
void rosa_mstr_reserve_cap_inc(rosa_mstr* dst, size_t min_inc_cap);

void rosa_mstr_shrink_fit(rosa_mstr* dst);

void rosa_mstr_insert(rosa_mstr* dst, rosa_mstr* src, size_t insert_pos);

void rosa_mstr_replace_char(rosa_mstr* dst, size_t replace_pos, char src);

void rosa_mstr_erase(rosa_mstr* dst, rosa_mstr* src, size_t erase_pos, size_t erase_len);

//TODO return proper sorting..
int rosa_mstr_compare(rosa_mstr* cmp1, rosa_mstr* cmp2);

bool rosa_mstr_startswith(rosa_mstr* dst, rosa_mstr* src);
bool rosa_mstr_endswith(rosa_mstr* dst, rosa_mstr* src);
bool rosa_mstr_contains(rosa_mstr* dst, rosa_mstr* src);

void rosa_mstr_swap(rosa_mstr* swap1, rosa_mstr* swap2);

//TODO print printf vprintf

//TODO hash

//TODO better names for flags
bool rosa_mstr_read_only(rosa_mstr* dst);
bool rosa_mstr_is_view(rosa_mstr* dst);

//TODO strutil strcpy managed-strings

//TODO copy, resize(setlen)?

//TODO conversions to various data types

#ifdef __cplusplus
}
#endif
