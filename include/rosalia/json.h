#ifndef ROSALIA_JSON_H_INCLUDE
#define ROSALIA_JSON_H_INCLUDE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_JSON_STATIC
#define ROSALIA__JSON_DEC static
#define ROSALIA__JSON_DEC_EXT static
#define ROSALIA__JSON_DEF static
#else
#define ROSALIA__JSON_DEC
#define ROSALIA__JSON_DEC_EXT extern
#define ROSALIA__JSON_DEF
#endif

#define ROSALIA_JSON_VERSION_MAJOR 0
#define ROSALIA_JSON_VERSION_MINOR 1
#define ROSALIA_JSON_VERSION_PATCH 1

#ifdef __cplusplus
extern "C" {
#endif

/* two very nice c json implementations:
https://github.com/DaveGamble/cJSON
https://github.com/json-parser/json-parser + builder
*/

//NOTE: this depends on noise.h, as "rosalia/noise.h"

typedef enum CJ_TYPE_E {
    CJ_TYPE_NONE = 0,
    CJ_TYPE_OBJECT,
    CJ_TYPE_ARRAY,
    CJ_TYPE_VNULL, // this is json null, read value-null
    CJ_TYPE_U64, //TODO should be signed int64
    CJ_TYPE_F32,
    CJ_TYPE_BOOL,
    CJ_TYPE_STRING,
    CJ_TYPE_ERROR,
    CJ_TYPE_COUNT,
    CJ_TYPE_MAX = UINT8_MAX,
} CJ_TYPE;

// static buffer string with capacity
typedef struct cj_sb_string_s {
    size_t cap;
    char* str;
} cj_sb_string;

typedef struct cj_color4u_s {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} cj_color4u;

typedef struct cj_color4f_s {
    float r;
    float g;
    float b;
    float a;
} cj_color4f;

typedef union cj_value_u {
    uint64_t u64;
    float f32;
    bool b;
    cj_sb_string s;
} cj_value;

// config object/value/array-container
typedef struct cj_ovac_s cj_ovac;

struct cj_ovac_s {
    cj_ovac* parent;

    // only valid if object/array
    uint32_t child_cap;
    uint32_t child_count;
    cj_ovac** children; //TODO maybe want to store the actual children instead of pointers here?
    //TODO use proper hashmap for object access? (additionaly, obj children are ordered so the plain array will always exist too!)

    //TODO could use ref counting and pointer to newer version here

    // only valid if parent is an object
    // if this is a child of an object, its key is o_label_str and o_label_hash=strhash(o_label_str)
    char* label_str;
    uint32_t label_hash; //TODO could malloc this side of the children to accelerate searching times in wide objects

    CJ_TYPE type;
    cj_value v;
};

//TODO file api for loading whole file to membuffer and writing whole file from membuffer

// operation on inplace sb_strings
void cj_sb_string_create(cj_sb_string* sbs, size_t cap, const char* str); // str is optional, and can be left NULL, use cap 0 to either infer the strlen or init as NULL is no str is given, if cap is non zero the initial str will be truncated if necessary
void cj_sb_string_resize(cj_sb_string* sbs, size_t cap, bool can_shrink);
void cj_sb_string_destroy(cj_sb_string* sbs);

// if str is an ovac of type vnull, retype it to sb_string, if cap > 0 then automaticall creates the internal string as well otherwise init to NULL
void cj_str_assume(cj_ovac* str, size_t cap);

cj_color4u cj_color4u_read(const char* str);
cj_color4f cj_color4f_read(const char* str);
// str must have capacity of at least 7 characters (9 if write_alpha is true)
void cj_color4u_write(char* str, cj_color4u c4u, bool write_alpha);
void cj_color4f_write(char* str, cj_color4f c4f, bool write_alpha);

cj_ovac* cj_create_object(uint32_t cap);
void cj_object_append(cj_ovac* obj, const char* key, cj_ovac* ovac);
//TODO cj_object_insert by idx?
cj_ovac* cj_object_get(cj_ovac* obj, const char* key); // returns the ovac found for this key under this obj, if it exists
void cj_object_replace(cj_ovac* obj, const char* key, cj_ovac* new_ovac);
cj_ovac* cj_object_detach(cj_ovac* obj, const char* key);
void cj_object_remove(cj_ovac* obj, const char* key);

cj_ovac* cj_create_vnull();
cj_ovac* cj_create_u64(uint64_t value);
cj_ovac* cj_create_f32(float value);
cj_ovac* cj_create_bool(bool value);
cj_ovac* cj_create_str(size_t cap, const char* str); // copies str into self, cap must be >= strlen(str)+1 //TODO make this more accessible: version with implicit cap and version without an actual string, i.e. null

cj_ovac* cj_create_array(uint32_t cap);
void cj_array_append(cj_ovac* arr, cj_ovac* ovac);
void cj_array_insert(cj_ovac* arr, uint32_t idx, cj_ovac* ovac); // insert such that child is reachable at idx in the array //TODO out of bounds behaviour? fill with vnull
void cj_array_replace(cj_ovac* arr, uint32_t idx, cj_ovac* new_ovac);
cj_ovac* cj_array_detach(cj_ovac* arr, uint32_t idx);
void cj_array_remove(cj_ovac* arr, uint32_t idx);

cj_ovac* cj_ovac_duplicate(cj_ovac* ovac);

// replace the type and value (i.e. children for obj/arr) of entry_ovac with that of data_ovac, but keeps entry_ovac stable as viewed from its parent, this consumes data ovac, effectively destroying it
void cj_ovac_replace(cj_ovac* entry_ovac, cj_ovac* data_ovac);

void cj_ovac_destroy(cj_ovac* ovac); // if ovac is in a parent container, it will automatically be removed from there first

// if str_hint is true then "\!XXX" and "\0" are enabled for use in strings
size_t cj_measure(cj_ovac* ovac, bool packed, bool str_hint); // includes a final zero terminator, returns 0 if a measure error occured, or ovac is NULL
char* cj_serialize(char* buf, cj_ovac* ovac, bool packed, bool str_hint); // returns a pointer just beyond the last written character, or NULL if a serialization error occured (e.g. invalid types) //TODO NO trailing commas!!
cj_ovac* cj_deserialize(const char* buf, bool str_hint); // must be zero terminated, returns an obj ovac if successful, otherwise an ovac of type CJ_TYPE_ERROR, which has a string value containing the error string, or NULL if no error specified

// cj_find uses a data path api to find the specified entry in the ovac (json) tree
// examples:
// "client.global.palette.wood_dark" returns a color4u ovac
// "client.plugins.loadlist[0]" returns the sb_str ovac which is the first element of the loadlist array
// for arrays one can also use [-1] to get the last element
//WARNING array indices for set not currently supported, future feature only
// when using set, array indices also have more utilizations:
// [0] replaces element at idx 0, [+] appends to the list, [+0] makes available at idx 0 but preserves all other elements
// for delete there is also [*] to delete all elements, equivalent to setting parent to an empty list
//TODO '[' that are part of a label string (key in an obj) need to be escaped accessed via thing["subkey\[text"], use normal escaping for '"' and '\'
cj_ovac* cj_find(cj_ovac* root, const char* data_path);
// cj_ovac* cj_set(cj_ovac* root, const char* data_path, cj_ovac* ovac, bool overwrite); // behaves like cj_find + cj_ovac_replace
// cj_ovac* cj_remove(cj_ovac* root, const char* data_path); // behaves like cj_find + cj_ovac_destroy
//TODO general detached, duplicate, replace?

// same as find but only return a shallow value copy, return true if successfully written to rv
// check vnull to see if it was value null, optional (if not existant then function returns false on vnull b/c no value was written to rv)
bool cj_get_u64(cj_ovac* root, const char* data_path, uint64_t* rv, bool* vnull);
bool cj_get_f32(cj_ovac* root, const char* data_path, float* rv, bool* vnull);
bool cj_get_bool(cj_ovac* root, const char* data_path, bool* rv, bool* vnull);
bool cj_get_str(cj_ovac* root, const char* data_path, cj_sb_string** rv, bool* vnull);
// shallow color get assumes data_path leads to a string ovac, this is then parsed as a color
bool cj_get_c4u(cj_ovac* root, const char* data_path, cj_color4u* rv, bool* vnull);
bool cj_get_c4f(cj_ovac* root, const char* data_path, cj_color4f* rv, bool* vnull);

// default get, same as get but returns the supplied default value if the datapath does not return a valid result
// i.e. if this returns false there was no value, and the value in rv is dv
bool cj_dget_u64(cj_ovac* root, const char* data_path, uint64_t* rv, uint64_t dv, bool* vnull);
bool cj_dget_f32(cj_ovac* root, const char* data_path, float* rv, float dv, bool* vnull);
bool cj_dget_bool(cj_ovac* root, const char* data_path, bool* rv, bool dv, bool* vnull);
bool cj_dget_str(cj_ovac* root, const char* data_path, cj_sb_string** rv, cj_sb_string* dv, bool* vnull);
bool cj_dget_c4u(cj_ovac* root, const char* data_path, cj_color4u* rv, cj_color4u dv, bool* vnull);
bool cj_dget_c4f(cj_ovac* root, const char* data_path, cj_color4f* rv, cj_color4f dv, bool* vnull);

//TODO impl
// force value set, makes v available under data_path, if required, new objects will be inserted and arrays filled with vnulls
// careful: this destroys and overrides previous data available at the data_path or partials of it
// returns false if the data_path did not already match the desired types
// bool cj_fset_vnull(cj_ovac* root, const char* data_path);
// bool cj_fset_u64(cj_ovac* root, const char* data_path, uint64_t v);
// bool cj_fset_f32(cj_ovac* root, const char* data_path, uint64_t v);
// bool cj_fset_bool(cj_ovac* root, const char* data_path, uint64_t v);
// bool cj_fset_str(cj_ovac* root, const char* data_path, uint64_t v);
// bool cj_fset_c4u(cj_ovac* root, const char* data_path, uint64_t v);
// bool cj_fset_c4f(cj_ovac* root, const char* data_path, uint64_t v);

#ifdef __cplusplus
}
#endif

#endif

//TODO serialization.h compatibility

#if defined(ROSALIA_JSON_IMPLEMENTATION) && !defined(ROSALIA_JSON_H_IMPL)
#define ROSALIA_JSON_H_IMPL

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROSALIA_NOISE_STATIC
#define ROSALIA_NOISE_IMPLEMENTATION
#include "rosalia/noise.h"

#ifdef __cplusplus
extern "C" {
#endif

static const uint32_t CJ_BASE_CAP = 4;

void cj_sb_string_create(cj_sb_string* sbs, size_t cap, const char* str)
{
    sbs->cap = cap;
    if (cap == 0 && str == NULL) {
        sbs->str = NULL;
        return;
    }
    if (cap == 0) {
        sbs->cap = strlen(str) + 1;
    }
    sbs->str = (char*)malloc(sbs->cap);
    if (str != NULL) {
        strncpy(sbs->str, str, sbs->cap - 1);
    }
}

void cj_sb_string_resize(cj_sb_string* sbs, size_t cap, bool can_shrink)
{
    if (cap == sbs->cap) {
        return;
    }
    sbs->cap = cap;
    if (cap == 0) {
        if (sbs->str != NULL) {
            free(sbs->str);
        }
        return;
    }
    if (can_shrink == false && cap < sbs->cap) {
        return;
    }
    char* new_str = (char*)malloc(cap);
    strncpy(new_str, sbs->str, cap - 1);
    if (sbs->str != NULL) {
        free(sbs->str);
    }
    sbs->str = new_str;
}

void cj_sb_string_destroy(cj_sb_string* sbs)
{
    if (sbs->str != NULL) {
        free(sbs->str);
    }
    sbs->cap = 0;
    sbs = NULL;
}

void cj_str_assume(cj_ovac* str, size_t cap)
{
    if (str->type == CJ_TYPE_VNULL) {
        str->type = CJ_TYPE_STRING;
        str->v.s = (cj_sb_string){
            .cap = cap,
            .str = cap > 0 ? (char*)malloc(cap) : NULL,
        };
    }
}

cj_color4u cj_color4u_read(const char* str)
{
    cj_color4u ret;
    int ec = sscanf(str, "#%02hhx%02hhx%02hhx%02hhx", &ret.r, &ret.g, &ret.b, &ret.a);
    if (ec == 4) {
        return ret;
    } else if (ec == 3) {
        ret.a = UINT8_MAX;
        return ret;
    }
    return (cj_color4u){.r = 0, .g = 0, .b = 0, .a = UINT8_MAX};
}

cj_color4f cj_color4f_read(const char* str)
{
    cj_color4u c4u = cj_color4u_read(str);
    return (cj_color4f){
        .r = (float)c4u.r / UINT8_MAX,
        .g = (float)c4u.g / UINT8_MAX,
        .b = (float)c4u.b / UINT8_MAX,
        .a = (float)c4u.a / UINT8_MAX,
    };
}

void cj_color4u_write(char* str, cj_color4u c4u, bool write_alpha)
{
    if (write_alpha == true) {
        sprintf(str, "#%02hhx%02hhx%02hhx%02hhx", c4u.r, c4u.g, c4u.b, c4u.a);
    } else {
        sprintf(str, "#%02hhx%02hhx%02hhx", c4u.r, c4u.g, c4u.b);
    }
}

void cj_color4f_write(char* str, cj_color4f c4f, bool write_alpha)
{
    cj_color4u_write(
        str,
        (cj_color4u){
            .r = (uint8_t)(c4f.r * UINT8_MAX),
            .g = (uint8_t)(c4f.g * UINT8_MAX),
            .b = (uint8_t)(c4f.b * UINT8_MAX),
            .a = (uint8_t)(c4f.a * UINT8_MAX),
        },
        write_alpha
    );
}

cj_ovac* cj_create_object(uint32_t cap)
{
    cj_ovac* obj = (cj_ovac*)malloc(sizeof(cj_ovac));
    *obj = (cj_ovac){
        .parent = NULL,
        .child_cap = cap,
        .child_count = 0,
        .children = (cap > 0 ? (cj_ovac**)malloc(sizeof(cj_ovac*) * cap) : NULL),
        .label_str = NULL,
        .label_hash = 0,
        .type = CJ_TYPE_OBJECT,
    };
    return obj;
}

void cj_object_append(cj_ovac* obj, const char* key, cj_ovac* ovac)
{
    if (obj->child_cap == 0) {
        obj->child_cap = CJ_BASE_CAP;
        obj->children = (cj_ovac**)malloc(sizeof(cj_ovac*) * obj->child_cap);
    }
    if (obj->child_count + 1 > obj->child_cap) {
        obj->child_cap *= 2;
        cj_ovac** children_new = (cj_ovac**)malloc(sizeof(cj_ovac*) * obj->child_cap);
        memcpy(children_new, obj->children, sizeof(cj_ovac*) * obj->child_count);
        free(obj->children);
        obj->children = children_new;
    }
    obj->children[obj->child_count++] = ovac;
    ovac->parent = obj;
    ovac->label_hash = strhash(key, NULL);
    ovac->label_str = strdup(key);
}

cj_ovac* cj_object_get(cj_ovac* obj, const char* key)
{
    uint32_t key_hash = strhash(key, NULL);
    for (uint32_t idx = 0; idx < obj->child_count; idx++) {
        cj_ovac* cp = obj->children[idx];
        if (cp->label_hash == key_hash && strcmp(cp->label_str, key) == 0) {
            return cp;
        }
    }
    return NULL;
}

void cj_object_replace(cj_ovac* obj, const char* key, cj_ovac* new_ovac)
{
    cj_ovac_replace(cj_object_get(obj, key), new_ovac);
}

cj_ovac* cj_object_detach(cj_ovac* obj, const char* key)
{
    uint32_t key_hash = strhash(key, NULL);
    for (uint32_t idx = 0; idx < obj->child_count; idx++) {
        cj_ovac* cp = obj->children[idx];
        if (cp->label_hash == key_hash && strcmp(cp->label_str, key) == 0) {
            cp->parent = NULL;
            memmove(obj->children + idx, obj->children + idx + 1, sizeof(cj_ovac*) * obj->child_count - idx - 1);
            if (obj->child_count < obj->child_cap / 4) {
                // resize obj cap if it falls too low
                obj->child_cap /= 2;
                cj_ovac** children_new = (cj_ovac**)malloc(sizeof(cj_ovac*) * obj->child_cap);
                memcpy(children_new, obj->children, sizeof(cj_ovac*) * obj->child_count);
                obj->children = children_new;
            }
            return cp;
        }
    }
    return NULL;
}

void cj_object_remove(cj_ovac* obj, const char* key)
{
    uint32_t key_hash = strhash(key, NULL);
    for (uint32_t idx = 0; idx < obj->child_count; idx++) {
        cj_ovac* cp = obj->children[idx];
        if (cp->label_hash == key_hash && strcmp(cp->label_str, key) == 0) {
            cp->parent = NULL;
            cj_ovac_destroy(cp);
            memmove(obj->children + idx, obj->children + idx + 1, sizeof(cj_ovac*) * obj->child_count - idx - 1);
            if (obj->child_count < obj->child_cap / 4) {
                // resize obj cap if it falls too low
                obj->child_cap /= 2;
                cj_ovac** children_new = (cj_ovac**)malloc(sizeof(cj_ovac*) * obj->child_cap);
                memcpy(children_new, obj->children, sizeof(cj_ovac*) * obj->child_count);
                obj->children = children_new;
            }
            return;
        }
    }
}

cj_ovac* cj_create_vnull()
{
    cj_ovac* val = (cj_ovac*)malloc(sizeof(cj_ovac));
    *val = (cj_ovac){
        .parent = NULL,
        .child_cap = 0,
        .child_count = 0,
        .children = 0,
        .label_str = NULL,
        .label_hash = 0,
        .type = CJ_TYPE_VNULL,
    };
    return val;
}

cj_ovac* cj_create_u64(uint64_t value)
{
    cj_ovac* val = (cj_ovac*)malloc(sizeof(cj_ovac));
    *val = (cj_ovac){
        .parent = NULL,
        .child_cap = 0,
        .child_count = 0,
        .children = 0,
        .label_str = NULL,
        .label_hash = 0,
        .type = CJ_TYPE_U64,
        .v = (cj_value){
            .u64 = value,
        },
    };
    return val;
}

cj_ovac* cj_create_f32(float value)
{
    cj_ovac* val = (cj_ovac*)malloc(sizeof(cj_ovac));
    *val = (cj_ovac){
        .parent = NULL,
        .child_cap = 0,
        .child_count = 0,
        .children = 0,
        .label_str = NULL,
        .label_hash = 0,
        .type = CJ_TYPE_F32,
        .v = (cj_value){
            .f32 = value,
        },
    };
    return val;
}

cj_ovac* cj_create_bool(bool value)
{
    cj_ovac* val = (cj_ovac*)malloc(sizeof(cj_ovac));
    *val = (cj_ovac){
        .parent = NULL,
        .child_cap = 0,
        .child_count = 0,
        .children = 0,
        .label_str = NULL,
        .label_hash = 0,
        .type = CJ_TYPE_BOOL,
        .v = (cj_value){
            .b = value,
        },
    };
    return val;
}

cj_ovac* cj_create_str(size_t cap, const char* str)
{
    cj_ovac* val = (cj_ovac*)malloc(sizeof(cj_ovac));
    *val = (cj_ovac){
        .parent = NULL,
        .child_cap = 0,
        .child_count = 0,
        .children = 0,
        .label_str = NULL,
        .label_hash = 0,
        .type = CJ_TYPE_STRING,
        .v = (cj_value){
            .s = (cj_sb_string){
                .cap = cap,
                .str = (char*)malloc(cap),
            },
        },
    };
    strcpy(val->v.s.str, str);
    return val;
}

cj_ovac* cj_create_array(uint32_t cap)
{
    cj_ovac* arr = (cj_ovac*)malloc(sizeof(cj_ovac));
    *arr = (cj_ovac){
        .parent = NULL,
        .child_cap = cap,
        .child_count = 0,
        .children = (cap > 0 ? (cj_ovac**)malloc(sizeof(cj_ovac*) * cap) : NULL),
        .label_str = NULL,
        .label_hash = 0,
        .type = CJ_TYPE_ARRAY,
    };
    return arr;
}

void cj_array_append(cj_ovac* arr, cj_ovac* ovac)
{
    if (arr->child_cap == 0) {
        arr->child_cap = CJ_BASE_CAP;
        arr->children = (cj_ovac**)malloc(sizeof(cj_ovac*) * arr->child_cap);
    }
    if (arr->child_count + 1 > arr->child_cap) {
        arr->child_cap *= 2;
        cj_ovac** children_new = (cj_ovac**)malloc(sizeof(cj_ovac*) * arr->child_cap);
        memcpy(children_new, arr->children, sizeof(cj_ovac*) * arr->child_count);
        free(arr->children);
        arr->children = children_new;
    }
    arr->children[arr->child_count++] = ovac;
    ovac->parent = arr;
}

void cj_array_insert(cj_ovac* arr, uint32_t idx, cj_ovac* ovac)
{
    //TODO out of bounds behaviour: for now assumes child_count <= idx, could also fill with vnull on too high?
    if (arr->child_cap == 0) {
        arr->child_cap = CJ_BASE_CAP;
        arr->children = (cj_ovac**)malloc(sizeof(cj_ovac*) * arr->child_cap);
    }
    if (arr->child_count + 1 > arr->child_cap) {
        arr->child_cap *= 2;
        cj_ovac** children_new = (cj_ovac**)malloc(sizeof(cj_ovac*) * arr->child_cap);
        memcpy(children_new, arr->children, sizeof(cj_ovac*) * arr->child_count);
        arr->children = children_new;
    }
    //TODO move this memmove up into the if, so its only 2 of half size instead of one large and 1 half
    memmove(arr->children + idx + 1, arr->children + idx, sizeof(cj_ovac*) * (arr->child_count - idx));
    arr->children[idx] = ovac;
    arr->child_count++;
    ovac->parent = arr;
}

void cj_array_replace(cj_ovac* arr, uint32_t idx, cj_ovac* new_ovac)
{
    cj_ovac_replace(arr->children[idx], new_ovac);
}

cj_ovac* cj_array_detach(cj_ovac* arr, uint32_t idx)
{
    if (idx >= arr->child_count) {
        return NULL;
    }
    cj_ovac* cp = arr->children[idx];
    cp->parent = NULL;
    memmove(arr->children + idx, arr->children + idx + 1, sizeof(cj_ovac*) * arr->child_count - idx - 1);
    if (arr->child_count < arr->child_cap / 4) {
        // resize arr cap if it falls too low
        arr->child_cap /= 2;
        cj_ovac** children_new = (cj_ovac**)malloc(sizeof(cj_ovac*) * arr->child_cap);
        memcpy(children_new, arr->children, sizeof(cj_ovac*) * arr->child_count);
        arr->children = children_new;
    }
    return cp;
}

void cj_array_remove(cj_ovac* arr, uint32_t idx)
{
    arr->children[idx]->parent = NULL;
    cj_ovac_destroy(arr->children[idx]);
    memmove(arr->children + idx, arr->children + idx + 1, sizeof(cj_ovac*) * arr->child_count - idx - 1);
    if (arr->child_count < arr->child_cap / 4) {
        // resize arr cap if it falls too low
        arr->child_cap /= 2;
        cj_ovac** children_new = (cj_ovac**)malloc(sizeof(cj_ovac*) * arr->child_cap);
        memcpy(children_new, arr->children, sizeof(cj_ovac*) * arr->child_count);
        arr->children = children_new;
    }
}

cj_ovac* cj_ovac_duplicate(cj_ovac* ovac)
{
    cj_ovac* clone = (cj_ovac*)malloc(sizeof(cj_ovac));
    switch (ovac->type) {
        case CJ_TYPE_NONE: {
            assert(0);
        } break;
        case CJ_TYPE_OBJECT:
        case CJ_TYPE_ARRAY: {
            *clone = (cj_ovac){
                .parent = NULL,
                .child_cap = ovac->child_cap,
                .child_count = ovac->child_count,
                .children = (cj_ovac**)malloc(sizeof(cj_ovac*) * ovac->child_cap),
                .label_str = (ovac->label_str ? strdup(ovac->label_str) : NULL),
                .label_hash = ovac->label_hash,
                .type = ovac->type,
            };
            for (uint32_t idx = 0; idx < ovac->child_count; idx++) {
                clone->children[idx] = cj_ovac_duplicate(ovac->children[idx]); //TODO make this iterative instead of recursive
            }
        } break;
        case CJ_TYPE_VNULL:
        case CJ_TYPE_U64:
        case CJ_TYPE_F32:
        case CJ_TYPE_BOOL: {
            *clone = (cj_ovac){
                .parent = NULL,
                .child_cap = 0,
                .child_count = 0,
                .children = 0,
                .label_str = (ovac->label_str ? strdup(ovac->label_str) : NULL),
                .label_hash = ovac->label_hash,
                .type = ovac->type,
                .v = ovac->v,
            };
        } break;
        case CJ_TYPE_STRING: {
            //TODO clone for NULL strings
            *clone = (cj_ovac){
                .parent = NULL,
                .child_cap = 0,
                .child_count = 0,
                .children = 0,
                .label_str = (ovac->label_str ? strdup(ovac->label_str) : NULL),
                .label_hash = ovac->label_hash,
                .type = CJ_TYPE_STRING,
                .v = (cj_value){
                    .s = (cj_sb_string){
                        .cap = ovac->v.s.cap,
                        .str = (char*)malloc(ovac->v.s.cap),
                    },
                },
            };
            strcpy(clone->v.s.str, ovac->v.s.str);
        } break;
        case CJ_TYPE_ERROR: {
            assert(0); //TODO rather error?
        } break;
        case CJ_TYPE_COUNT:
        case CJ_TYPE_MAX: {
            assert(0);
        } break;
    }
    return clone;
}

void cj_ovac_replace(cj_ovac* entry_ovac, cj_ovac* data_ovac)
{
    if (entry_ovac->children != NULL) {
        // destroy all children of entry ovac, better way?
        for (uint32_t idx = 0; idx < entry_ovac->child_count; idx++) {
            entry_ovac->children[idx]->parent = NULL;
            cj_ovac_destroy(entry_ovac->children[idx]);
        }
        free(entry_ovac->children);
    }
    entry_ovac->child_cap = data_ovac->child_cap;
    entry_ovac->child_count = data_ovac->child_count;
    entry_ovac->children = data_ovac->children;
    entry_ovac->type = data_ovac->type;
    entry_ovac->v = data_ovac->v;
    if (data_ovac->label_str != NULL) {
        free(data_ovac->label_str);
    }
    free(data_ovac);
}

void cj_ovac_destroy(cj_ovac* ovac)
{
    if (ovac->parent != NULL) {
        // remove from parent, automitcally deletes the subtree
        switch (ovac->parent->type) {
            case CJ_TYPE_OBJECT: {
                cj_object_detach(ovac->parent, ovac->label_str);
            } break;
            case CJ_TYPE_ARRAY: {
                //TODO if idx in parent gets added accelerate finding the child for removal here
                for (uint32_t idx = 0; idx < ovac->parent->child_count; idx++) {
                    if (ovac->parent->children[idx] == ovac) {
                        cj_array_detach(ovac->parent, idx);
                        break;
                    }
                }
            } break;
            default: {
                assert(0);
            } break;
        }
    }
    if (ovac->children != NULL) {
        for (uint32_t idx = 0; idx < ovac->child_count; idx++) {
            ovac->children[idx]->parent = NULL;
            cj_ovac_destroy(ovac->children[idx]); //TODO make this iterative instead of recursive
        }
        free(ovac->children);
    }
    if (ovac->label_str != NULL) {
        free(ovac->label_str);
    }
    if (ovac->type == CJ_TYPE_STRING) {
        free(ovac->v.s.str); //BUG can str be null here?
    }
    free(ovac);
}

size_t cj_measure_impl(cj_ovac* ovac, bool packed, uint32_t depth, bool str_hint)
{
    size_t acc = 0;
    if (ovac->label_str != NULL) {
        acc += 3 + strlen(ovac->label_str) + (packed ? 0 : 1); // "\"%s\": "
    }
    switch (ovac->type) {
        case CJ_TYPE_NONE: {
            return 0;
        } break;
        case CJ_TYPE_OBJECT:
        case CJ_TYPE_ARRAY: {
            acc += 2; // "{" and "}" or "[" or "]"
            if (ovac->child_count > 0) {
                if (packed == false) {
                    acc += 1 + 4 * depth; // "\n" and "    " for every child
                }
                for (uint32_t i = 0; i < ovac->child_count; i++) {
                    if (packed == false) {
                        acc += 4 * (depth + 1) + 1; // padding for child and "\n" since unpacked
                    }
                    acc += cj_measure_impl(ovac->children[i], packed, depth + 1, str_hint);
                    if (packed == false || i < ovac->child_count - 1) {
                        acc += 1; // "," at end of child
                    }
                }
            }
        } break;
        case CJ_TYPE_VNULL: {
            acc += 4;
        } break;
        case CJ_TYPE_U64: {
            acc += snprintf(NULL, 0, "%lu", ovac->v.u64);
        } break;
        case CJ_TYPE_F32: {
            acc += snprintf(NULL, 0, "%f", ovac->v.f32);
        } break;
        case CJ_TYPE_BOOL: {
            acc += ovac->v.b ? 4 : 5;
        } break;
        case CJ_TYPE_STRING: {
            if (ovac->v.s.str == NULL) {
                if (str_hint == true) {
                    acc += 5; // for null hint and its quotes: "\!0"
                } else {
                    acc += 4; // for "null"
                }
            } else {
                acc += 2 + strlen(ovac->v.s.str); // "\"%s\"" //TODO lots of encoding issues
            }
        } break;
        case CJ_TYPE_ERROR: {
            return 0;
        } break;
        case CJ_TYPE_COUNT: {
            case CJ_TYPE_MAX:
                return 0;
        } break;
    }
    if (depth == 0) {
        acc += 2; // "\n" and "\0" for root obj
    }
    return acc;
}

size_t cj_measure(cj_ovac* ovac, bool packed, bool str_hint)
{
    if (ovac == NULL) {
        return 0;
    }
    return cj_measure_impl(ovac, packed, 0, str_hint);
}

char* cj_serialize_impl(char* buf, cj_ovac* ovac, bool packed, uint32_t depth, bool str_hint)
{
    char* wrp = buf;
    if (ovac->label_str != NULL) {
        wrp += sprintf(wrp, "\"%s\":", ovac->label_str);
        if (packed == false) {
            wrp += sprintf(wrp, " ");
        }
    }
    switch (ovac->type) {
        case CJ_TYPE_NONE: {
            return NULL;
        } break;
        case CJ_TYPE_OBJECT:
        case CJ_TYPE_ARRAY: {
            switch (ovac->type) {
                case CJ_TYPE_OBJECT: {
                    wrp += sprintf(wrp, "{");
                } break;
                case CJ_TYPE_ARRAY: {
                    wrp += sprintf(wrp, "[");
                } break;
                default: {
                    assert(0);
                } break;
            }
            if (ovac->child_count > 0) {
                if (packed == false) {
                    wrp += sprintf(wrp, "\n");
                }
                for (uint32_t i = 0; i < ovac->child_count; i++) {
                    if (packed == false) {
                        memset(wrp, ' ', 4 * (depth + 1));
                        wrp += 4 * (depth + 1);
                    }
                    wrp = cj_serialize_impl(wrp, ovac->children[i], packed, depth + 1, str_hint);
                    if (packed == false || i < ovac->child_count - 1) {
                        wrp += sprintf(wrp, ",");
                    }
                    if (packed == false) {
                        wrp += sprintf(wrp, "\n");
                    }
                }
                if (packed == false) {
                    memset(wrp, ' ', 4 * depth);
                    wrp += 4 * depth;
                }
            }
            switch (ovac->type) {
                case CJ_TYPE_OBJECT: {
                    wrp += sprintf(wrp, "}");
                } break;
                case CJ_TYPE_ARRAY: {
                    wrp += sprintf(wrp, "]");
                } break;
                default: {
                    assert(0);
                } break;
            }
        } break;
        case CJ_TYPE_VNULL: {
            wrp += sprintf(wrp, "null");
        } break;
        case CJ_TYPE_U64: {
            wrp += sprintf(wrp, "%lu", ovac->v.u64);
        } break;
        case CJ_TYPE_F32: {
            wrp += sprintf(wrp, "%f", ovac->v.f32);
        } break;
        case CJ_TYPE_BOOL: {
            wrp += sprintf(wrp, ovac->v.b ? "true" : "false");
        } break;
        case CJ_TYPE_STRING: {
            if (ovac->v.s.str == NULL) {
                if (str_hint == true) {
                    wrp += sprintf(wrp, "\"\\!0\"");
                } else {
                    wrp += sprintf(wrp, "null");
                }
            } else {
                wrp += sprintf(wrp, "\"%s\"", ovac->v.s.str); //TODO lots of encoding issues
            }
        } break;
        case CJ_TYPE_ERROR: {
            return NULL;
        } break;
        case CJ_TYPE_COUNT:
        case CJ_TYPE_MAX: {
            return NULL;
        } break;
    }
    if (depth == 0) {
        wrp += sprintf(wrp, "\n");
    }
    return wrp;
}

char* cj_serialize(char* buf, cj_ovac* ovac, bool packed, bool str_hint)
{
    if (ovac == NULL) {
        return NULL;
    }
    return cj_serialize_impl(buf, ovac, packed, 0, str_hint); // recursive implementation
    // iterative implementation: easily without extra stack if children of node are stored raw not as pointers, then every node automatically knows it s idx in the parent container by (this - this->parent.children) b/c this points inside the children array
}

cj_ovac* cj_deserialize_impl_abort(cj_ovac* ovac, size_t lnum, size_t cnum, const char* fmt, ...)
{
    if (ovac != NULL) {
        // destroy ovac
        cj_ovac* dp = ovac;
        while (dp->parent) {
            dp = dp->parent;
        }
        cj_ovac_destroy(dp);
    }
    // return a formatted error
    if (fmt == NULL) {
        return NULL;
    }
    char* estr;
    va_list args;
    va_start(args, fmt);
    size_t len = snprintf(NULL, 0, "%zu:%zu: ", lnum, cnum) + vsnprintf(NULL, 0, fmt, args) + 1;
    va_end(args);
    estr = (char*)malloc(len);
    if (estr == NULL) {
        return NULL;
    }
    cj_ovac* rp = cj_create_str(len, "");
    rp->type = CJ_TYPE_ERROR;
    char* rp_str = rp->v.s.str + sprintf(rp->v.s.str, "%zu:%zu: ", lnum, cnum);
    va_start(args, fmt);
    vsnprintf(rp_str, len, fmt, args);
    va_end(args);
    return rp;
}

//TODO allow \t as whitespace
// allows for block comments "/*...*/", which may never interrupt values, but can otherwise appear anywhere
cj_ovac* cj_deserialize(const char* buf, bool str_hint)
{
    const char* wbuf = buf;

    size_t lnum = 0;
    size_t cnum = 0;

    bool want_colon = false;
    bool want_comma = false;
    bool want_value = true;

    cj_ovac* ccon = NULL; // current container (object/array)
    cj_sb_string pstr = (cj_sb_string){.cap = 0, .str = NULL};

    char c = *(wbuf++);
    while (c != '\0') {
        switch (c) {
            case '/': {
                if (*wbuf != '*') {
                    break;
                }
                size_t e_lnum = lnum;
                size_t e_cnum = cnum;
                wbuf++;
                cnum++;
                // skip forward until end of comment
                const char* cbuf = wbuf;
                while (*cbuf != '\0' && (*cbuf != '*' || *(cbuf + 1) != '/')) {
                    cnum++;
                    if (*cbuf == '\n') {
                        lnum++;
                        cnum = 0;
                    }
                    cbuf++;
                }
                if (*cbuf == '*') {
                    cbuf += 2;
                    cnum += 2;
                } else {
                    return cj_deserialize_impl_abort(ccon, e_lnum, e_cnum, "unterminated block comment");
                }
                wbuf = cbuf;
            } break;
            case ' ': {
                // pass
            } break;
            case '\n': {
                lnum++;
            } break;
            case '{':
            case '[': {
                cj_ovac* new_con;
                if (c == '{') {
                    new_con = cj_create_object(0);
                    want_value = false;
                } else {
                    new_con = cj_create_array(0);
                }
                if (ccon != NULL) {
                    if (ccon->type == CJ_TYPE_OBJECT) {
                        cj_object_append(ccon, pstr.str, new_con);
                        pstr.cap = 0;
                        free(pstr.str);
                        pstr.str = NULL;
                    } else if (ccon->type == CJ_TYPE_ARRAY) {
                        cj_array_append(ccon, new_con);
                    } else {
                        assert(0); // this can never happen
                    }
                }
                new_con->parent = ccon;
                ccon = new_con;
            } break;
            case '}':
            case ']': {
                // close container
                if (ccon == NULL) {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "can not close missing container");
                }
                if (c == '}' && ccon->type != CJ_TYPE_OBJECT) {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected '}' on closing array");
                } else if (c == ']' && ccon->type != CJ_TYPE_ARRAY) {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected ']' on closing object");
                }
                if (want_colon == true || (ccon->type == CJ_TYPE_OBJECT && want_value == true)) {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "illegal key without value on closing object");
                }
                if (ccon->parent == NULL) {
                    // check that only whitespace follows, then return parsed ovac
                    while (*wbuf != '\0' && (*wbuf == ' ' || *wbuf == '\n')) {
                        cnum++;
                        if (*wbuf == '\n') {
                            lnum++;
                            cnum = 0;
                        }
                        wbuf++;
                    }
                    if (*wbuf == '\0') {
                        return ccon;
                    } else {
                        return cj_deserialize_impl_abort(ccon, lnum, cnum, "illegal non whitespace content after main content");
                    }
                }
                ccon = ccon->parent;
                want_comma = true;
                want_value = (ccon->type == CJ_TYPE_ARRAY);
            } break;
            case '"': {
                //TODO validate that all codepoints are valid utf-8
                // parse the string currently at buf
                cj_sb_string val_str;
                size_t str_len = 0;
                size_t cap_res = SIZE_MAX;
                size_t e_lnum = lnum;
                size_t e_cnum = cnum;
                const char* cbuf = wbuf;
                while (*cbuf != '\0' && *cbuf != '\"' && *cbuf != '\n') {
                    if (*cbuf == '\\') {
                        cbuf++;
                        switch (*cbuf) {
                            case '!': { // custom string hint escape code, must be the last thing in the json string
                                if (str_hint == false) {
                                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected string capacity reservation hint, disallowed by runtime");
                                }
                                if (ccon && pstr.str == NULL && ccon->type == CJ_TYPE_OBJECT) {
                                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "string capacity reservation only allowed in value strings");
                                }
                                int scan_len;
                                int ec = sscanf(cbuf, "!%zu%n", &cap_res, &scan_len);
                                if (ec != 1) {
                                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "invalid string capacity reservation");
                                }
                                if (cap_res == 0) {
                                    if (cbuf > wbuf + 1) {
                                        return cj_deserialize_impl_abort(ccon, lnum, cnum, "illegal null string hint in non null string");
                                    }
                                }
                                cbuf += scan_len - 1;
                                cnum += scan_len - 1;
                                if (*(cbuf + 1) != '\"') {
                                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "invalid string capacity reservation is not final in string");
                                }
                                str_len--;
                            } break;
                            case '0': { // additional sanity escape code for zero terminator
                                if (str_hint == false) {
                                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected zero terminator string hint, disallowed by runtime");
                                }
                                if (ccon && pstr.str == NULL && ccon->type == CJ_TYPE_OBJECT) {
                                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "zero terminator only allowed in value strings");
                                }
                                str_len--;
                            } break;
                            // normal escapes: " \ / b f n r t
                            case '\"':
                            case '\\':
                            case '/':
                            case 'b':
                            case 'f':
                            case 'n':
                            case 'r':
                            case 't': {
                                // pass
                            } break;
                            case 'u': {
                                // explicit unicode point
                                //TODO check for validity
                                cbuf += 3;
                                cnum += 3;
                            } break;
                            default: {
                                return cj_deserialize_impl_abort(ccon, lnum, cnum, "illegal escape character: %c (%hhu)", *cbuf, (uint8_t)*cbuf);
                            } break;
                        }
                        cnum++;
                    }
                    str_len++;
                    cbuf++;
                    cnum++;
                }
                if (*cbuf == '\0') {
                    return cj_deserialize_impl_abort(ccon, e_lnum, e_cnum, "unterminated string");
                } else if (*cbuf == '\n') {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "raw newline in string");
                }
                if (cap_res > 0) {
                    if (cap_res == SIZE_MAX) {
                        // no size hint given, use detected length
                        cap_res = str_len + 1;
                    } else if (cap_res < str_len + 1) {
                        // increase cap_res
                        cap_res = str_len + 1;
                    }
                    val_str.cap = cap_res;
                    val_str.str = (char*)malloc(cap_res);
                    if (val_str.str == NULL) {
                        return cj_deserialize_impl_abort(ccon, lnum, cnum, NULL);
                    }
                    // need to memcpy each char individually in extra loop to allow for escape codes
                    size_t copy_idx = 0;
                    while (*wbuf != '\"') {
                        if (*wbuf == '\\') {
                            wbuf++;
                            switch (*wbuf) {
                                case '!': { // custom string hint escape code
                                    while (*(wbuf + 1) != '\"') {
                                        wbuf++;
                                    }
                                } break;
                                case '0': { // additional sanity escape code for zero terminator
                                    val_str.str[copy_idx] = '\0';
                                } break;
                                // normal escapes: " \ / b f n r t
                                case '\"': {
                                    val_str.str[copy_idx] = '\"';
                                } break;
                                case '\\': {
                                    val_str.str[copy_idx] = '\\';
                                } break;
                                case '/': {
                                    val_str.str[copy_idx] = '/';
                                } break;
                                case 'b': {
                                    val_str.str[copy_idx] = '\b';
                                } break;
                                case 'f': {
                                    val_str.str[copy_idx] = '\f';
                                } break;
                                case 'n': {
                                    val_str.str[copy_idx] = '\n';
                                } break;
                                case 'r': {
                                    val_str.str[copy_idx] = '\r';
                                } break;
                                case 't': {
                                    val_str.str[copy_idx] = '\t';
                                } break;
                                case 'u': {
                                    // explicit unicode point
                                    //TODO
                                    wbuf += 3;
                                } break;
                                default: {
                                    assert(0); // handled in first loop
                                } break;
                            }
                        } else {
                            val_str.str[copy_idx] = *wbuf;
                        }
                        wbuf++;
                        copy_idx++;
                    }
                    val_str.str[str_len] = '\0';
                } else {
                    // value string is hinted null
                    val_str.cap = 0;
                    val_str.str = NULL;
                }
                wbuf = cbuf + 1;
                cnum++;
                // process parsed string
                if (ccon != NULL && pstr.str == NULL && ccon->type == CJ_TYPE_OBJECT) {
                    // this a key
                    pstr = val_str;
                    want_colon = true;
                    want_value = false;
                } else {
                    // this is a value string
                    cj_ovac* new_str;
                    if (val_str.str == NULL) {
                        new_str = cj_create_vnull();
                        new_str->type = CJ_TYPE_STRING;
                        new_str->v.s = (cj_sb_string){.cap = 0, .str = NULL};
                    } else {
                        new_str = cj_create_str(val_str.cap, val_str.str);
                        free(val_str.str);
                    }
                    if (ccon == NULL) {
                        // root value instead of obj/arr
                        return new_str;
                    }
                    if (pstr.str != NULL) {
                        cj_object_append(ccon, pstr.str, new_str);
                        pstr.cap = 0;
                        free(pstr.str);
                        pstr.str = NULL;
                        want_value = false;
                    } else {
                        cj_array_append(ccon, new_str);
                        want_value = true;
                    }
                    want_comma = true;
                }
            } break;
            case ':': {
                if (want_colon == true) {
                    want_colon = false;
                    want_value = true;
                } else {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected ':'");
                }
            } break;
            case ',': {
                if (want_comma == true) {
                    want_comma = false;
                    want_value = (ccon->type == CJ_TYPE_ARRAY);
                } else {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected ','");
                }
            } break;
            default: {
                if (want_comma == true) {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected value, missing ','");
                }
                if (want_colon == true) {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected value, missing ':'");
                }
                if (want_value == false) {
                    return cj_deserialize_impl_abort(ccon, lnum, cnum, "unexpected value, missing key");
                }
                // value: vnull, u64, f32, bool
                cj_ovac* new_val = NULL;
                uint64_t u64;
                float f32;
                int vlen = 1;
                int scan_len;
                size_t e_lnum = lnum;
                size_t e_cnum = cnum;
                const char* cbuf = wbuf;
                // skip forward to next non value character //TODO //BUG make this proper
                while (*cbuf != '\0' && *cbuf != ',' && *cbuf != ' ' && *cbuf != '\n' && *cbuf != '}' && *cbuf != ']') {
                    cnum++;
                    if (*wbuf == '\n') {
                        lnum++;
                        cnum = 0;
                    }
                    cbuf++;
                }
                vlen += cbuf - wbuf;
                if (strncmp(wbuf - 1, "null", 4) == 0) {
                    new_val = cj_create_vnull();
                } else if (strncmp(wbuf - 1, "true", 4) == 0) {
                    new_val = cj_create_bool(true);
                } else if (strncmp(wbuf - 1, "false", 5) == 0) {
                    new_val = cj_create_bool(false);
                } else if (sscanf(wbuf - 1, "%lu%n", &u64, &scan_len) == 1 && scan_len == vlen) {
                    new_val = cj_create_u64(u64);
                } else if (sscanf(wbuf - 1, "%f%n", &f32, &scan_len) == 1 && scan_len == vlen) {
                    new_val = cj_create_f32(f32);
                } else {
                    return cj_deserialize_impl_abort(ccon, e_lnum, e_cnum, "invalid value type");
                }
                wbuf = cbuf;
                if (ccon == NULL) {
                    // root value instead of obj/arr
                    return new_val;
                }
                // append value to container
                if (pstr.str != NULL) {
                    cj_object_append(ccon, pstr.str, new_val);
                    pstr.cap = 0;
                    free(pstr.str);
                    pstr.str = NULL;
                } else {
                    cj_array_append(ccon, new_val);
                }
                want_comma = true;
                want_value = (ccon->type == CJ_TYPE_ARRAY);
            } break;
        }
        c = *(wbuf++);
        cnum++;
    }
    if (ccon == NULL) {
        return cj_deserialize_impl_abort(ccon, lnum, cnum, "no content");
    } else {
        return cj_deserialize_impl_abort(ccon, lnum, cnum, "unterminated root container");
    }
}

cj_ovac* cj_find(cj_ovac* root, const char* data_path)
{
    cj_ovac* ccon = root;
    const char* rbuf = data_path;

    bool want_dot = false;

    char c = *(rbuf++);
    while (c != '\0') {
        switch (c) {
            case '.': {
                if (want_dot == false) {
                    return NULL;
                }
                want_dot = false;
            } break;
            case '[': {
                if (want_dot == true) {
                    return NULL;
                }
                // array indexing
                if (ccon->type != CJ_TYPE_ARRAY) {
                    return NULL;
                }
                int64_t scan_child_idx;
                int scan_len;
                int ec = sscanf(rbuf, "%li]%n", &scan_child_idx, &scan_len);
                if (ec != 1) {
                    return NULL;
                }
                uint32_t child_idx = (scan_child_idx < 0 ? ccon->child_count - (uint32_t)(-scan_child_idx) : (uint32_t)scan_child_idx);
                if (child_idx >= ccon->child_count) {
                    return NULL;
                }
                ccon = ccon->children[child_idx];
                rbuf = rbuf + scan_len;
                want_dot = (ccon->type == CJ_TYPE_OBJECT);
            } break;
            default: {
                if (want_dot == true) {
                    return NULL;
                }
                // normal object key
                if (ccon->type != CJ_TYPE_OBJECT) {
                    return NULL;
                }
                rbuf--;
                const char* key_end = rbuf;
                while (*key_end != '\0' && *key_end != '.' && *key_end != '[') {
                    key_end++;
                }
                cj_ovac* new_con = NULL;
                {
                    //WARNING this break with new obj find key impls
                    uint32_t key_hash = strhash(rbuf, key_end);
                    for (uint32_t idx = 0; idx < ccon->child_count; idx++) {
                        cj_ovac* cp = ccon->children[idx];
                        if (cp->label_hash == key_hash && strncmp(cp->label_str, rbuf, key_end - rbuf) == 0) {
                            new_con = cp;
                            break;
                        }
                    }
                }
                if (new_con == NULL) {
                    return NULL;
                }
                ccon = new_con;
                rbuf = key_end;
                want_dot = (ccon->type == CJ_TYPE_OBJECT);
            } break;
        }
        c = *(rbuf++);
    }
    return ccon;
}

bool cj_get_u64(cj_ovac* root, const char* data_path, uint64_t* rv, bool* vnull)
{
    if (vnull != NULL) {
        *vnull = false;
    }
    cj_ovac* ovac = cj_find(root, data_path);
    if (ovac == NULL) {
        return false;
    }
    if (ovac->type == CJ_TYPE_VNULL) {
        if (vnull != NULL) {
            *vnull = true;
        }
        return false;
    }
    if (ovac->type != CJ_TYPE_U64) {
        return false;
    }
    *rv = ovac->v.u64;
    return true;
}

bool cj_get_f32(cj_ovac* root, const char* data_path, float* rv, bool* vnull)
{
    if (vnull != NULL) {
        *vnull = false;
    }
    cj_ovac* ovac = cj_find(root, data_path);
    if (ovac == NULL) {
        return false;
    }
    if (ovac->type == CJ_TYPE_VNULL) {
        if (vnull != NULL) {
            *vnull = true;
        }
        return false;
    }
    if (ovac->type != CJ_TYPE_F32) {
        return false;
    }
    *rv = ovac->v.f32;
    return true;
}

bool cj_get_bool(cj_ovac* root, const char* data_path, bool* rv, bool* vnull)
{
    if (vnull != NULL) {
        *vnull = false;
    }
    cj_ovac* ovac = cj_find(root, data_path);
    if (ovac == NULL) {
        return false;
    }
    if (ovac->type == CJ_TYPE_VNULL) {
        if (vnull != NULL) {
            *vnull = true;
        }
        return false;
    }
    if (ovac->type != CJ_TYPE_BOOL) {
        return false;
    }
    *rv = ovac->v.b;
    return true;
}

bool cj_get_str(cj_ovac* root, const char* data_path, cj_sb_string** rv, bool* vnull)
{
    if (vnull != NULL) {
        *vnull = false;
    }
    cj_ovac* ovac = cj_find(root, data_path);
    if (ovac == NULL) {
        return false;
    }
    if (ovac->type == CJ_TYPE_VNULL) {
        if (vnull != NULL) {
            *vnull = true;
        }
        return false;
    }
    if (ovac->type != CJ_TYPE_STRING) {
        return false;
    }
    *rv = &ovac->v.s;
    return true;
}

bool cj_get_c4u(cj_ovac* root, const char* data_path, cj_color4u* rv, bool* vnull)
{
    if (vnull != NULL) {
        *vnull = false;
    }
    cj_ovac* ovac = cj_find(root, data_path);
    if (ovac == NULL) {
        return false;
    }
    if (ovac->type == CJ_TYPE_VNULL) {
        if (vnull != NULL) {
            *vnull = true;
        }
        return false;
    }
    if (ovac->type != CJ_TYPE_STRING || ovac->v.s.cap < 7) {
        return false;
    }
    *rv = cj_color4u_read(ovac->v.s.str);
    return true;
}

bool cj_get_c4f(cj_ovac* root, const char* data_path, cj_color4f* rv, bool* vnull)
{
    if (vnull != NULL) {
        *vnull = false;
    }
    cj_ovac* ovac = cj_find(root, data_path);
    if (ovac == NULL) {
        return false;
    }
    if (ovac->type == CJ_TYPE_VNULL) {
        if (vnull != NULL) {
            *vnull = true;
        }
        return false;
    }
    if (ovac->type != CJ_TYPE_STRING || ovac->v.s.cap < 7) {
        return false;
    }
    *rv = cj_color4f_read(ovac->v.s.str);
    return true;
}

bool cj_dget_u64(cj_ovac* root, const char* data_path, uint64_t* rv, uint64_t dv, bool* vnull)
{
    bool found = cj_get_u64(root, data_path, rv, vnull);
    if (found == false) {
        *rv = dv;
    }
    return found;
}

bool cj_dget_f32(cj_ovac* root, const char* data_path, float* rv, float dv, bool* vnull)
{
    bool found = cj_get_f32(root, data_path, rv, vnull);
    if (found == false) {
        *rv = dv;
    }
    return found;
}

bool cj_dget_bool(cj_ovac* root, const char* data_path, bool* rv, bool dv, bool* vnull)
{
    bool found = cj_get_bool(root, data_path, rv, vnull);
    if (found == false) {
        *rv = dv;
    }
    return found;
}

bool cj_dget_str(cj_ovac* root, const char* data_path, cj_sb_string** rv, cj_sb_string* dv, bool* vnull)
{

    bool found = cj_get_str(root, data_path, rv, vnull);
    if (found == false) {
        *rv = dv;
    }
    return found;
}

bool cj_dget_c4u(cj_ovac* root, const char* data_path, cj_color4u* rv, cj_color4u dv, bool* vnull)
{
    bool found = cj_get_c4u(root, data_path, rv, vnull);
    if (found == false) {
        *rv = dv;
    }
    return found;
}

bool cj_dget_c4f(cj_ovac* root, const char* data_path, cj_color4f* rv, cj_color4f dv, bool* vnull)
{
    bool found = cj_get_c4f(root, data_path, rv, vnull);
    if (found == false) {
        *rv = dv;
    }
    return found;
}

#ifdef __cplusplus
}
#endif

#endif
