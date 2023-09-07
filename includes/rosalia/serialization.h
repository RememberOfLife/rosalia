#ifndef ROSALIA_SERIALIZATION_H_INCLUDE
#define ROSALIA_SERIALIZATION_H_INCLUDE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_SERIALIZATION_STATIC
#define ROSALIA__SERIALIZATION_DEC static
#define ROSALIA__SERIALIZATION_DEC_EXT static
#define ROSALIA__SERIALIZATION_DEF static
#else
#define ROSALIA__SERIALIZATION_DEC
#define ROSALIA__SERIALIZATION_DEC_EXT extern
#define ROSALIA__SERIALIZATION_DEF
#endif

#ifdef ROSALIA_SERIALIZATION_DECORATE
#define ROSALIA__SERIALIZATION_DECORATE(ident) ROSALIA_SERIALIZATION_DECORATE(ident)
#else
#define ROSALIA__SERIALIZATION_DECORATE(ident) ident
#endif

#define ROSALIA_SERIALIZATION_VERSION_MAJOR 0
#define ROSALIA_SERIALIZATION_VERSION_MINOR 6
#define ROSALIA_SERIALIZATION_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

//TODO unserializable type that throws error on serialize and copy
//TODO api helpers to write easy custom serializer functions if the type is known
//TODO more detailed error reporting?
//TODO maybe max depth for recursive ptr types
//TODO how does size work across machines of different register width?
//TODO cyclic pointer graph serialization via pointer cache and index, i.e. dfs/bfs serialize graph and instead of pointers with recursive serialization, put an index into the pointer cache list, on deserialize first allocate all objects in the pointercache then replace indices by pointers //TODO this needs more serialization state than just initzero/destroy can provide (where to remember pointercache?) //TODO this should work for graphs where we just have one entry pointer, and also for those where we get the array of all objects in advance //TODO might want to support multiple distinct entry pointers, e.g. pointers to objects additionally stored as hashmap value somewhere (is this even doable with the scheme?)
//TODO functionally tagged unions, dont use a map but use a function for mapping enums values to layouts instead

//TODO decorate structs/enums/functypes

//TODO global accessible bools so everyone can check if features like vector or hm are implemented as well or not

//NOTE: sparse enums for union tags not supported for now

/////
// raw stream utils

//TODO ROSALIA__SERIALIZATION_DECORATE this struct
typedef struct raw_stream_s {
    uint8_t* begin;
    uint8_t* end;
} raw_stream;

ROSALIA__SERIALIZATION_DEC raw_stream ROSALIA__SERIALIZATION_DECORATE(rs_init)(uint8_t* buf);
ROSALIA__SERIALIZATION_DEC size_t ROSALIA__SERIALIZATION_DECORATE(rs_get_size)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_bool)(raw_stream* rs, bool v);
ROSALIA__SERIALIZATION_DEC bool ROSALIA__SERIALIZATION_DECORATE(rs_r_bool)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_int8)(raw_stream* rs, int8_t v);
ROSALIA__SERIALIZATION_DEC int8_t ROSALIA__SERIALIZATION_DECORATE(rs_r_int8)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_int16)(raw_stream* rs, int16_t v);
ROSALIA__SERIALIZATION_DEC int16_t ROSALIA__SERIALIZATION_DECORATE(rs_r_int16)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_int32)(raw_stream* rs, int32_t v);
ROSALIA__SERIALIZATION_DEC int32_t ROSALIA__SERIALIZATION_DECORATE(rs_r_int32)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_int64)(raw_stream* rs, int64_t v);
ROSALIA__SERIALIZATION_DEC int64_t ROSALIA__SERIALIZATION_DECORATE(rs_r_int64)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_uint8)(raw_stream* rs, uint8_t v);
ROSALIA__SERIALIZATION_DEC uint8_t ROSALIA__SERIALIZATION_DECORATE(rs_r_uint8)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_uint16)(raw_stream* rs, uint16_t v);
ROSALIA__SERIALIZATION_DEC uint16_t ROSALIA__SERIALIZATION_DECORATE(rs_r_uint16)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_uint32)(raw_stream* rs, uint32_t v);
ROSALIA__SERIALIZATION_DEC uint32_t ROSALIA__SERIALIZATION_DECORATE(rs_r_uint32)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_uint64)(raw_stream* rs, uint64_t v);
ROSALIA__SERIALIZATION_DEC uint64_t ROSALIA__SERIALIZATION_DECORATE(rs_r_uint64)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_size)(raw_stream* rs, size_t v);
ROSALIA__SERIALIZATION_DEC size_t ROSALIA__SERIALIZATION_DECORATE(rs_r_size)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_float)(raw_stream* rs, float v);
ROSALIA__SERIALIZATION_DEC float ROSALIA__SERIALIZATION_DECORATE(rs_r_float)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_double)(raw_stream* rs, double v);
ROSALIA__SERIALIZATION_DEC double ROSALIA__SERIALIZATION_DECORATE(rs_r_double)(raw_stream* rs);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_string)(raw_stream* rs, const char* str, const char* str_end);
ROSALIA__SERIALIZATION_DEC size_t ROSALIA__SERIALIZATION_DECORATE(rs_r_string_size)(raw_stream* rs); // does NOT alter the stream position; DOES account for the null character
ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_r_string)(raw_stream* rs, char* str);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_w_raw)(raw_stream* rs, const uint8_t* buf, size_t size);
ROSALIA__SERIALIZATION_DEC size_t ROSALIA__SERIALIZATION_DECORATE(rs_r_raw_size)(raw_stream* rs); // does NOT alter the stream position
ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(rs_r_raw)(raw_stream* rs, uint8_t* buf);

/////
// utils for serialization

ROSALIA__SERIALIZATION_DEC void* ROSALIA__SERIALIZATION_DECORATE(ptradd)(void* p, size_t v);
ROSALIA__SERIALIZATION_DEC void* ROSALIA__SERIALIZATION_DECORATE(ptrsub)(void* p, size_t v);
ROSALIA__SERIALIZATION_DEC size_t ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(const void* p_end, const void* p_start);

typedef struct blob_s {
    size_t len;
    void* data;
} blob;

ROSALIA__SERIALIZATION_DEC_EXT const blob BLOB_NULL;

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(blob_create)(blob* b, size_t len);

ROSALIA__SERIALIZATION_DEC bool ROSALIA__SERIALIZATION_DECORATE(blob_is_null)(blob* b);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(blob_resize)(blob* b, size_t len, bool preserve_data);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(blob_copy)(blob* t, blob* s);

ROSALIA__SERIALIZATION_DEC void ROSALIA__SERIALIZATION_DECORATE(blob_destroy)(blob* b);

/////
// layout based serialization

//NOTE: complex types like uint32_t** ptr_to_ptrarray; require a separate layout for the uint32_t* serialization of the uint32_t* elems in the ptrarray

typedef enum SL_TYPE_E {
    SL_TYPE_NULL = 0,

    // primitives
    SL_TYPE_BOOL,
    SL_TYPE_I8,
    SL_TYPE_I16,
    SL_TYPE_I32,
    SL_TYPE_I64,
    SL_TYPE_U8,
    SL_TYPE_U16,
    SL_TYPE_U32,
    SL_TYPE_U64,
    SL_TYPE_SIZE,
    SL_TYPE_FLOAT,
    SL_TYPE_DOUBLE,

    // pseudo primitives
    SL_TYPE_STRING, // char* member;
    SL_TYPE_BLOB, // blob member;
    //TODO both blob and string need some way to set max size

    // extension types, must supply typesize if ptr/arr
    SL_TYPE_UNION_INTERNALLY_TAGGED, // use ext.un.{tag_size,tag_max,tag_map}
    SL_TYPE_UNION_EXTERNALLY_TAGGED, // use ext.un.{tag_size,tag_offset,tag_max,tag_map} and make sure to add a serialization layout entry to serialize the tag YOURSELF, this layout entry MUST be BEFORE the entry for the union and it MUST be on the same hierarchy level that the union is
    SL_TYPE_COMPLEX, // use ext.layout to specify the type //TODO rename to struct/layout?
    SL_TYPE_CUSTOM, // use ext.serializer to supply the serializer to use

    SL_TYPE_STOP, // signals the end of serialization items in this sl

    SL_TYPE_COUNT,

    SL_TYPE_TYPEMASK = 0x00FF,
    SL_TYPE_FLAGMASK = 0xFF00,
    // these are flags applicable to all serializable types
    // if not otherwise noted these flags are mututally exclusive with each other to provide ease of use
    SL_TYPE_PTR = 1 << (8 + 0), // this and SL_TYPE_ARRAY are compatible with each other
    SL_TYPE_ARRAY = 1 << (8 + 1), // if ptr: use len.offset else: len.immediate, to get size of the array, len.offset always locates a size_t
    SL_TYPE_PTRARRAY = SL_TYPE_PTR | SL_TYPE_ARRAY, // convenience type for sl definitions
    // pseudo flags
    // SL_TYPE_VECTOR = 1 << (8 + 2), //TODO impl
    //TODO more pseudo types, and how are things like hm with multiple types made?

    SL_TYPE_SIZE_MAX = UINT16_MAX,
} SL_TYPE;

#define SIZEOF_MEMBER(s, m) sizeof(((s*)0)->m)
// clang-format off
// use SL_TYPE_UAUTOP2 to deduce unsigned power of two sizes, e.g. for enum values which are not packed as expected
#define SL_TYPE_UAUTOP2(s, m) (SIZEOF_MEMBER(s, m) == 8 ? SL_TYPE_U64 : SIZEOF_MEMBER(s, m) == 4 ? SL_TYPE_U32 : SIZEOF_MEMBER(s, m) == 2 ? SL_TYPE_U16 : SIZEOF_MEMBER(s, m) == 1 ? SL_TYPE_U8 : SL_TYPE_SIZE_MAX)

// clang-format on

// general serializer invocation type
typedef enum GSIT_E {
    GSIT_NONE,
    GSIT_INITZERO, // supply obj_in as object to initalize to zero (similar to destroy)
    GSIT_SIZE, // size from obj_in
    GSIT_SERIALIZE, // serialize obj_in
    GSIT_DESERIALIZE, // deserialize to obj_out
    GSIT_COPY, // copy from obj_in to obj_out
    GSIT_DESTROY, // destroy obj_in
    GSIT_COUNT,
    GSIT_SIZE_MAX = UINT8_MAX,
} GSIT;

typedef struct serialization_layout_s serialization_layout;

// must provide guarantee that any zero initialized object will be destructible from any partial deserialized state *as left by this function*
typedef size_t custom_serializer_t(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end);

//TODO dont use size_t for offsets and sizes because they are so small anyways?
struct serialization_layout_s {
    SL_TYPE type;
    size_t data_offset;

    union {
        const serialization_layout* layout;
        // size_t serializer(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
        custom_serializer_t* serializer;

        struct {
            size_t tag_size; // may be {1,2,4,8}
            size_t tag_offset; // only for externally tagged unions, always exactly 0 for internally tagged unions, offset from top of containing struct
            size_t tag_max; // tag values are tightly packed from 0 to tag_max, where tag_max is the smallest illegal tag (i.e. count of valid tags)
            // the tag_map maps all possible values the serializer can se in the tag, to a serialization layout
            // externally tagged unions supply NULL here for values where the union does not require any member to be serialized
            // internally tagged unions supply the tag/base layout for values where *only this* and no other members are required
            const serialization_layout** tag_map;
        } un;
    } ext;

    size_t typesize; // req'd only if PTR || ARRAY

    union {
        size_t immediate;
        size_t offset;
        size_t max; // optional, only for ptrarray, ignored if 0 //TODO this should be outside of the union!
    } len; // req'd only if PTR || ARRAY

    //TODO typeopts void* which is passed verbatim to the type serializer, e.g. for int min max or blob max size etc
};

static const size_t LS_ERR = SIZE_MAX;

// returned size_t is only valid if itype SIZE/SERIALIZE/DESERIALIZE
// if return value is LS_ERR then an error occured, can not happen during init or destroy
// deserialization errors are automatically cleaned up and do not leak memory (assuming all used custom functions do this as well)
ROSALIA__SERIALIZATION_DEC size_t ROSALIA__SERIALIZATION_DECORATE(layout_serializer)(GSIT itype, const serialization_layout* layout, void* obj_in, void* obj_out, void* buf, void* buf_end);

//TODO rather give lookup array?
// primitive serializers
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_bool_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u8_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u16_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u32_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u64_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_size_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_float_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_double_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_string_serializer);
ROSALIA__SERIALIZATION_DEC custom_serializer_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_blob_serializer);

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_SERIALIZATION_IMPLEMENTATION) && !defined(ROSALIA_SERIALIZATION_H_IMPL)
#define ROSALIA_SERIALIZATION_H_IMPL

#define ROSALIA__SERIALIZATION_INTERNAL(ident) rosalia__serialization_internal_##ident

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/////
// raw stream utils

//TODO simplify RAW_STREAM encoding process for byte based primitives (int/uint/float etc) by just using one, "encode bytes" func, on a union type!

ROSALIA__SERIALIZATION_DEF raw_stream ROSALIA__SERIALIZATION_DECORATE(rs_init)(uint8_t* buf)
{
    return (raw_stream){
        .begin = buf,
        .end = buf,
    };
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(rs_get_size)(raw_stream* rs)
{
    return rs->end - rs->begin;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_bool)(raw_stream* rs, bool v)
{
    rs->end[0] = (uint8_t)v;
    rs->end++;
}

ROSALIA__SERIALIZATION_DEF bool ROSALIA__SERIALIZATION_DECORATE(rs_r_bool)(raw_stream* rs)
{
    bool r = (bool)(rs->end[0]);
    rs->end++;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_int8)(raw_stream* rs, int8_t v)
{
    rs->end[0] = (uint8_t)v;
    rs->end++;
}

ROSALIA__SERIALIZATION_DEF int8_t ROSALIA__SERIALIZATION_DECORATE(rs_r_int8)(raw_stream* rs)
{
    int8_t r = (int8_t)(rs->end[0]);
    rs->end++;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_int16)(raw_stream* rs, int16_t v)
{
    rs->end[0] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 2;
}

ROSALIA__SERIALIZATION_DEF int16_t ROSALIA__SERIALIZATION_DECORATE(rs_r_int16)(raw_stream* rs)
{
    int16_t r = 0;
    r |= (int16_t)(rs->end[0]) << 8;
    r |= (int16_t)(rs->end[1]) << 0;
    rs->end += 2;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_int32)(raw_stream* rs, int32_t v)
{
    rs->end[0] = (uint8_t)((v >> 24) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 16) & 0xFF);
    rs->end[2] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[3] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 4;
}

ROSALIA__SERIALIZATION_DEF int32_t ROSALIA__SERIALIZATION_DECORATE(rs_r_int32)(raw_stream* rs)
{
    int32_t r = 0;
    r |= (int32_t)(rs->end[0]) << 24;
    r |= (int32_t)(rs->end[1]) << 16;
    r |= (int32_t)(rs->end[2]) << 8;
    r |= (int32_t)(rs->end[3]) << 0;
    rs->end += 4;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_int64)(raw_stream* rs, int64_t v)
{
    rs->end[0] = (uint8_t)((v >> 56) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 48) & 0xFF);
    rs->end[2] = (uint8_t)((v >> 40) & 0xFF);
    rs->end[3] = (uint8_t)((v >> 32) & 0xFF);
    rs->end[4] = (uint8_t)((v >> 24) & 0xFF);
    rs->end[5] = (uint8_t)((v >> 16) & 0xFF);
    rs->end[6] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[7] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 8;
}

ROSALIA__SERIALIZATION_DEF int64_t ROSALIA__SERIALIZATION_DECORATE(rs_r_int64)(raw_stream* rs)
{
    int64_t r = 0;
    r |= (int64_t)(rs->end[0]) << 56;
    r |= (int64_t)(rs->end[1]) << 48;
    r |= (int64_t)(rs->end[2]) << 40;
    r |= (int64_t)(rs->end[3]) << 32;
    r |= (int64_t)(rs->end[4]) << 24;
    r |= (int64_t)(rs->end[5]) << 16;
    r |= (int64_t)(rs->end[6]) << 8;
    r |= (int64_t)(rs->end[7]) << 0;
    rs->end += 8;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_uint8)(raw_stream* rs, uint8_t v)
{
    rs->end[0] = (uint8_t)v;
    rs->end++;
}

ROSALIA__SERIALIZATION_DEF uint8_t ROSALIA__SERIALIZATION_DECORATE(rs_r_uint8)(raw_stream* rs)
{
    uint8_t r = (uint8_t)(rs->end[0]);
    rs->end++;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_uint16)(raw_stream* rs, uint16_t v)
{
    rs->end[0] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 2;
}

ROSALIA__SERIALIZATION_DEF uint16_t ROSALIA__SERIALIZATION_DECORATE(rs_r_uint16)(raw_stream* rs)
{
    uint16_t r = 0;
    r |= (uint16_t)(rs->end[0]) << 8;
    r |= (uint16_t)(rs->end[1]) << 0;
    rs->end += 2;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_uint32)(raw_stream* rs, uint32_t v)
{
    rs->end[0] = (uint8_t)((v >> 24) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 16) & 0xFF);
    rs->end[2] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[3] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 4;
}

ROSALIA__SERIALIZATION_DEF uint32_t ROSALIA__SERIALIZATION_DECORATE(rs_r_uint32)(raw_stream* rs)
{
    uint32_t r = 0;
    r |= (uint32_t)(rs->end[0]) << 24;
    r |= (uint32_t)(rs->end[1]) << 16;
    r |= (uint32_t)(rs->end[2]) << 8;
    r |= (uint32_t)(rs->end[3]) << 0;
    rs->end += 4;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_uint64)(raw_stream* rs, uint64_t v)
{
    rs->end[0] = (uint8_t)((v >> 56) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 48) & 0xFF);
    rs->end[2] = (uint8_t)((v >> 40) & 0xFF);
    rs->end[3] = (uint8_t)((v >> 32) & 0xFF);
    rs->end[4] = (uint8_t)((v >> 24) & 0xFF);
    rs->end[5] = (uint8_t)((v >> 16) & 0xFF);
    rs->end[6] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[7] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 8;
}

ROSALIA__SERIALIZATION_DEF uint64_t ROSALIA__SERIALIZATION_DECORATE(rs_r_uint64)(raw_stream* rs)
{
    uint64_t r = 0;
    r |= (uint64_t)(rs->end[0]) << 56;
    r |= (uint64_t)(rs->end[1]) << 48;
    r |= (uint64_t)(rs->end[2]) << 40;
    r |= (uint64_t)(rs->end[3]) << 32;
    r |= (uint64_t)(rs->end[4]) << 24;
    r |= (uint64_t)(rs->end[5]) << 16;
    r |= (uint64_t)(rs->end[6]) << 8;
    r |= (uint64_t)(rs->end[7]) << 0;
    rs->end += 8;
    return r;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_size)(raw_stream* rs, size_t v)
{
    ROSALIA__SERIALIZATION_DECORATE(rs_w_uint64)
    (rs, (uint64_t)(v));
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(rs_r_size)(raw_stream* rs)
{
    return (size_t)ROSALIA__SERIALIZATION_DECORATE(rs_r_uint64)(rs);
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_float)(raw_stream* rs, float v)
{
    union {
        float f;
        uint8_t u8[sizeof(float)];
    } f32 = {.f = v};

    for (int i = 0; i < sizeof(float); i++) {
        ROSALIA__SERIALIZATION_DECORATE(rs_w_uint8)
        (rs, f32.u8[i]);
    }
}

ROSALIA__SERIALIZATION_DEF float ROSALIA__SERIALIZATION_DECORATE(rs_r_float)(raw_stream* rs)
{
    union {
        float f;
        uint8_t u8[sizeof(float)];
    } f32;

    for (int i = 0; i < sizeof(float); i++) {
        f32.u8[i] = ROSALIA__SERIALIZATION_DECORATE(rs_r_uint8)(rs);
    }
    return f32.f;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_double)(raw_stream* rs, double v)
{
    union {
        double d;
        uint8_t u8[sizeof(double)];
    } f64 = {.d = v};

    for (int i = 0; i < sizeof(double); i++) {
        ROSALIA__SERIALIZATION_DECORATE(rs_w_uint8)
        (rs, f64.u8[i]);
    }
}

ROSALIA__SERIALIZATION_DEF double ROSALIA__SERIALIZATION_DECORATE(rs_r_double)(raw_stream* rs)
{
    union {
        double d;
        uint8_t u8[sizeof(double)];
    } f64;

    for (int i = 0; i < sizeof(double); i++) {
        f64.u8[i] = ROSALIA__SERIALIZATION_DECORATE(rs_r_uint8)(rs);
    }
    return f64.d;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_string)(raw_stream* rs, const char* str, const char* str_end)
{
    size_t size;
    if (str_end == NULL) {
        size = strlen(str) + 1;
    } else {
        size = str_end - str;
    }
    memcpy(rs->end, str, size);
    rs->end += size;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(rs_r_string_size)(raw_stream* rs)
{
    return strlen((char*)rs->end) + 1;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_r_string)(raw_stream* rs, char* str)
{
    size_t len = strlen((char*)rs->end) + 1;
    memcpy(str, rs->end, len);
    rs->end += len;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_w_raw)(raw_stream* rs, const uint8_t* buf, size_t size)
{
    ROSALIA__SERIALIZATION_DECORATE(rs_w_size)
    (rs, size);
    memcpy(rs->end, buf, size);
    rs->end += size;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(rs_r_raw_size)(raw_stream* rs)
{
    size_t size = ROSALIA__SERIALIZATION_DECORATE(rs_r_size)(rs);
    rs->end -= sizeof(uint64_t); // this function should not alter the stream position
    return size;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(rs_r_raw)(raw_stream* rs, uint8_t* buf)
{
    size_t size = ROSALIA__SERIALIZATION_DECORATE(rs_r_size)(rs);
    memcpy(buf, rs->end, size);
    rs->end += size;
}

/////
// utils for event serialization

ROSALIA__SERIALIZATION_DEF const blob BLOB_NULL = (blob){
    .len = 0,
    .data = NULL,
};

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(blob_create)(blob* b, size_t len)
{
    b->len = len;
    b->data = len > 0 ? malloc(len) : NULL;
}

ROSALIA__SERIALIZATION_DEF bool ROSALIA__SERIALIZATION_DECORATE(blob_is_null)(blob* b)
{
    assert(b->data == NULL);
    return b->len == 0;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(blob_resize)(blob* b, size_t len, bool preserve_data)
{
    if (b->len == len) {
        return;
    }
    blob new_blob;
    ROSALIA__SERIALIZATION_DECORATE(blob_create)
    (&new_blob, len);
    if (preserve_data == true && b->len > 0 && len > 0) {
        memcpy(new_blob.data, b->data, len);
    }
    ROSALIA__SERIALIZATION_DECORATE(blob_destroy)
    (b);
    *b = new_blob;
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(blob_copy)(blob* t, blob* s)
{
    blob_create(t, s->len);
    if (t->len > 0) {
        memcpy(t->data, s->data, t->len);
    }
}

ROSALIA__SERIALIZATION_DEF void ROSALIA__SERIALIZATION_DECORATE(blob_destroy)(blob* b)
{
    if (b->data) {
        free(b->data);
    }
    b->len = 0;
    b->data = NULL;
}

/////
// layout based general purpose serialization backend

//TODO can inline these, but instantiate https://stackoverflow.com/a/16245669

ROSALIA__SERIALIZATION_DEF void* ROSALIA__SERIALIZATION_DECORATE(ptradd)(void* p, size_t v)
{
    return (char*)p + v;
}

ROSALIA__SERIALIZATION_DEF void* ROSALIA__SERIALIZATION_DECORATE(ptrsub)(void* p, size_t v)
{
    return (char*)p - v;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(const void* p_end, const void* p_start)
{
    return (const char*)p_end - (const char*)p_start;
}

// primitive serialization functions

//TODO somehow make this less repetitive

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_bool_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    bool* cin_p = (bool*)obj_in;
    bool* cout_p = (bool*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            // pass
        } break;
        case GSIT_SIZE: {
            return 1;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_bool(&rs, *cin_p);
            return 1;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 1) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            *cout_p = rs_r_bool(&rs);
            return 1;
        } break;
        case GSIT_COPY: {
            *cout_p = *cin_p;
        } break;
        case GSIT_DESTROY: {
            // pass
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u8_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    uint8_t* cin_p = (uint8_t*)obj_in;
    uint8_t* cout_p = (uint8_t*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            // pass
        } break;
        case GSIT_SIZE: {
            return 1;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_uint8(&rs, *cin_p);
            return 1;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 1) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            *cout_p = rs_r_uint8(&rs);
            return 1;
        } break;
        case GSIT_COPY: {
            *cout_p = *cin_p;
        } break;
        case GSIT_DESTROY: {
            // pass
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u16_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    uint16_t* cin_p = (uint16_t*)obj_in;
    uint16_t* cout_p = (uint16_t*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            // pass
        } break;
        case GSIT_SIZE: {
            return 2;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_uint16(&rs, *cin_p);
            return 2;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 2) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            *cout_p = rs_r_uint16(&rs);
            return 2;
        } break;
        case GSIT_COPY: {
            *cout_p = *cin_p;
        } break;
        case GSIT_DESTROY: {
            // pass
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u32_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    uint32_t* cin_p = (uint32_t*)obj_in;
    uint32_t* cout_p = (uint32_t*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            // pass
        } break;
        case GSIT_SIZE: {
            return 4;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_uint32(&rs, *cin_p);
            return 4;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 4) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            *cout_p = rs_r_uint32(&rs);
            return 4;
        } break;
        case GSIT_COPY: {
            *cout_p = *cin_p;
        } break;
        case GSIT_DESTROY: {
            // pass
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u64_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    uint64_t* cin_p = (uint64_t*)obj_in;
    uint64_t* cout_p = (uint64_t*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            // pass
        } break;
        case GSIT_SIZE: {
            return 8;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_uint64(&rs, *cin_p);
            return 8;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 8) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            *cout_p = rs_r_uint64(&rs);
            return 8;
        } break;
        case GSIT_COPY: {
            *cout_p = *cin_p;
        } break;
        case GSIT_DESTROY: {
            // pass
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_size_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    size_t* cin_p = (size_t*)obj_in;
    size_t* cout_p = (size_t*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            // pass
        } break;
        case GSIT_SIZE: {
            return 8;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_size(&rs, *cin_p);
            return 8;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 8) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            *cout_p = rs_r_size(&rs);
            return 8;
        } break;
        case GSIT_COPY: {
            *cout_p = *cin_p;
        } break;
        case GSIT_DESTROY: {
            // pass
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_float_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    float* cin_p = (float*)obj_in;
    float* cout_p = (float*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            // pass
        } break;
        case GSIT_SIZE: {
            return 4;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_float(&rs, *cin_p);
            return 4;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 4) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            *cout_p = rs_r_float(&rs);
            return 4;
        } break;
        case GSIT_COPY: {
            *cout_p = *cin_p;
        } break;
        case GSIT_DESTROY: {
            // pass
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_double_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    double* cin_p = (double*)obj_in;
    double* cout_p = (double*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            // pass
        } break;
        case GSIT_SIZE: {
            return 8;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_double(&rs, *cin_p);
            return 8;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 8) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            *cout_p = rs_r_double(&rs);
            return 8;
        } break;
        case GSIT_COPY: {
            *cout_p = *cin_p;
        } break;
        case GSIT_DESTROY: {
            // pass
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_string_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    char** cin_p = (char**)obj_in;
    char** cout_p = (char**)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            *cin_p = NULL;
        } break;
        case GSIT_SIZE: {
            size_t string_size = (*cin_p == NULL ? 2 : strlen(*cin_p) + 1);
            return (string_size < 2 ? 2 : string_size);
        } break;
        case GSIT_SERIALIZE: {
            size_t string_size = (*cin_p == NULL ? 0 : strlen(*cin_p) + 1);
            // str_len is 1 if NULL or empty"", otherwise strlen+1
            if (string_size <= 1) {
                *(char*)buf = '\0';
                if (string_size == 0) {
                    *((uint8_t*)buf + 1) = 0x00; // NULL becomes 0x0000
                } else {
                    *((uint8_t*)buf + 1) = 0xFF; // empty"" becomes 0x00FF
                }
                string_size = 2;
            } else {
                memcpy(buf, *cin_p, string_size);
            }
            return string_size;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 1) {
                return LS_ERR;
            }
            size_t max_string_size = ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf);
            const void* found = memchr(buf, '\0', max_string_size);
            if (!found || max_string_size < 2) {
                return LS_ERR;
            }
            size_t string_size = ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(found, buf) + 1;
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < string_size) {
                return LS_ERR;
            }
            if (string_size == 1 && *(uint8_t*)ROSALIA__SERIALIZATION_DECORATE(ptradd)(buf, 1) == 0x00) {
                *cout_p = NULL;
                string_size = 2;
            } else {
                *cout_p = (char*)malloc(string_size);
                memcpy(*cout_p, buf, string_size);
                if (string_size == 1) {
                    string_size += 1;
                }
            }
            return string_size;
        } break;
        case GSIT_COPY: {
            *cout_p = (*cin_p != NULL ? strdup(*cin_p) : NULL);
        } break;
        case GSIT_DESTROY: {
            if (*cin_p != NULL) {
                free(*cin_p);
            }
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(ls_primitive_blob_serializer)(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    blob* cin_p = (blob*)obj_in;
    blob* cout_p = (blob*)obj_out;
    switch (itype) {
        case GSIT_NONE: {
            assert(0);
        } break;
        case GSIT_INITZERO: {
            *cin_p = (blob){
                .len = 0,
                .data = NULL,
            };
        } break;
        case GSIT_SIZE: {
            return 8 + cin_p->len;
        } break;
        case GSIT_SERIALIZE: {
            raw_stream rs = rs_init(buf);
            rs_w_size(&rs, cin_p->len);
            if (cin_p->len > 0) {
                memcpy(rs.end, cin_p->data, cin_p->len);
            }
            return 8 + cin_p->len;
        } break;
        case GSIT_DESERIALIZE: {
            if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < 8) {
                return LS_ERR;
            }
            raw_stream rs = rs_init(buf);
            size_t csize = rs_r_size(&rs);
            ROSALIA__SERIALIZATION_DECORATE(blob_create)
            (cout_p, csize);
            if (csize > 0) {
                if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, ROSALIA__SERIALIZATION_DECORATE(ptradd)(buf, 8)) < csize) {
                    return LS_ERR;
                }
                memcpy(cout_p->data, ROSALIA__SERIALIZATION_DECORATE(ptradd)(buf, 8), csize);
            }
            return csize + 8;
        } break;
        case GSIT_COPY: {
            ROSALIA__SERIALIZATION_DECORATE(blob_create)
            (cout_p, cin_p->len);
            memcpy(cout_p->data, cin_p->data, cin_p->len);
        } break;
        case GSIT_DESTROY: {
            ROSALIA__SERIALIZATION_DECORATE(blob_destroy)
            (cin_p);
        } break;
        case GSIT_COUNT:
        case GSIT_SIZE_MAX: {
            assert(0);
        } break;
    }
    return 0;
}

custom_serializer_t* ROSALIA__SERIALIZATION_INTERNAL(ls_primitive_serializers)[] = {
    [SL_TYPE_BOOL] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_bool_serializer),
    [SL_TYPE_I8] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u8_serializer),
    [SL_TYPE_I16] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u16_serializer),
    [SL_TYPE_I32] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u32_serializer),
    [SL_TYPE_I64] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u64_serializer),
    [SL_TYPE_U8] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u8_serializer),
    [SL_TYPE_U16] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u16_serializer),
    [SL_TYPE_U32] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u32_serializer),
    [SL_TYPE_U64] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_u64_serializer),
    [SL_TYPE_SIZE] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_size_serializer),
    [SL_TYPE_FLOAT] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_float_serializer),
    [SL_TYPE_DOUBLE] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_double_serializer),
    [SL_TYPE_STRING] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_string_serializer),
    [SL_TYPE_BLOB] = ROSALIA__SERIALIZATION_DECORATE(ls_primitive_blob_serializer),
};

//TODO replace surena/rawstream uses by something cheaper
// recursive object serializer
ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_INTERNAL(layout_serializer_impl)(GSIT itype, const serialization_layout* layout, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    assert(layout != NULL);
    const bool in_ex = (itype != GSIT_DESERIALIZE);
    assert(!(in_ex == true && obj_in == NULL));
    const bool out_ex = (itype == GSIT_DESERIALIZE || itype == GSIT_COPY);
    assert(!(out_ex == true && obj_out == NULL));
    const bool use_buf = (itype == GSIT_SERIALIZE || itype == GSIT_DESERIALIZE);
    assert(!(use_buf == true && buf == NULL));
    assert(!(itype == GSIT_DESERIALIZE && buf_end == NULL));
    size_t rsize = 0;
    const serialization_layout* pl = layout;
    void* cbuf = buf;
    void* ebuf = buf_end;
    while ((pl->type & SL_TYPE_TYPEMASK) != SL_TYPE_STOP) {
        const bool is_ptr = (pl->type & SL_TYPE_PTR);
        const bool is_array = (pl->type & SL_TYPE_ARRAY);
        size_t typesize = pl->typesize;
        switch (pl->type & SL_TYPE_TYPEMASK) {
            case SL_TYPE_BOOL: {
                typesize = sizeof(bool);
            } break;
            case SL_TYPE_U8: {
                typesize = sizeof(uint8_t);
            } break;
            case SL_TYPE_U16: {
                typesize = sizeof(uint16_t);
            } break;
            case SL_TYPE_U32: {
                typesize = sizeof(uint32_t);
            } break;
            case SL_TYPE_U64: {
                typesize = sizeof(uint64_t);
            } break;
            case SL_TYPE_SIZE: {
                typesize = sizeof(size_t);
            } break;
            case SL_TYPE_STRING: {
                typesize = sizeof(char*);
            } break;
            case SL_TYPE_BLOB: {
                typesize = sizeof(blob);
            } break;
        }
        assert(!((is_ptr == true || is_array == true) && typesize == 0)); // detect missing typesize if required
        assert(!(((pl->type & SL_TYPE_TYPEMASK) == SL_TYPE_COMPLEX && pl->ext.layout == NULL))); // detect missing layout info if required
        assert(!(((pl->type & SL_TYPE_TYPEMASK) == SL_TYPE_CUSTOM && pl->ext.serializer == NULL))); // detect missing serializer function if required
        assert(!(is_ptr == false && is_array == true && pl->len.immediate == 0)); // detect missing immediate array length
        void* in_p = NULL;
        void* out_p = NULL;
        if (in_ex == true) {
            in_p = ROSALIA__SERIALIZATION_DECORATE(ptradd)(obj_in, pl->data_offset);
        }
        if (out_ex == true) {
            out_p = ROSALIA__SERIALIZATION_DECORATE(ptradd)(obj_out, pl->data_offset);
        }
        size_t arr_len = 1;
        if (is_array == true) {
            if (is_ptr == true) {
                if (itype == GSIT_DESERIALIZE) {
                    if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(ebuf, cbuf) < 8) {
                        return LS_ERR;
                    }
                    raw_stream rs = rs_init(cbuf);
                    arr_len = rs_r_size(&rs);
                    cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, 8);
                } else {
                    arr_len = *(size_t*)ROSALIA__SERIALIZATION_DECORATE(ptradd)(obj_in, pl->len.offset);
                    if (use_buf == true) {
                        raw_stream rs = rs_init(cbuf);
                        rs_w_size(&rs, arr_len);
                        cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, 8);
                    }
                }
                if (out_ex) {
                    *(size_t*)ROSALIA__SERIALIZATION_DECORATE(ptradd)(obj_out, pl->len.offset) = arr_len;
                }
                rsize += 8;
                if (itype != GSIT_DESTROY && (pl->len.max > 0 && arr_len > pl->len.max)) {
                    return LS_ERR;
                }
            } else {
                arr_len = pl->len.immediate;
            }
        } else if (is_ptr == true) {
            if (itype == GSIT_DESERIALIZE) {
                if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(ebuf, cbuf) < 1) {
                    return LS_ERR;
                }
                raw_stream rs = rs_init(cbuf);
                arr_len = (rs_r_uint8(&rs) == 0xFF ? 1 : 0);
                cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, 1);
            } else {
                arr_len = ((*(void**)in_p) == NULL ? 0 : 1);
                if (use_buf == true) {
                    raw_stream rs = rs_init(cbuf);
                    rs_w_uint8(&rs, arr_len == 0 ? 0x00 : 0xFF);
                    cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, 1);
                }
            }
            rsize += 1;
        }
        if (is_ptr == true) {
            if (in_ex == true && arr_len > 0) {
                in_p = *(void**)in_p;
            }
            if (out_ex == true) {
                *(void**)out_p = (arr_len > 0 ? malloc(typesize * arr_len) : NULL);
                out_p = *(void**)out_p;
            }
        }
        for (size_t idx = 0; idx < arr_len; idx++) {
            SL_TYPE serialize_type = pl->type & SL_TYPE_TYPEMASK;
            assert(serialize_type > SL_TYPE_NULL && serialize_type < SL_TYPE_STOP); // cannot serialize unknown type
            //TODO assert: if ptr then is non null
            switch (serialize_type) {
                default: {
                    // use primitive serializer func
                    size_t csize = ROSALIA__SERIALIZATION_INTERNAL(ls_primitive_serializers)[serialize_type](itype, in_p, out_p, cbuf, ebuf);
                    if (csize == LS_ERR) {
                        return LS_ERR;
                    }
                    rsize += csize;
                    if (use_buf == true) {
                        cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, csize);
                    }
                } break;
                case SL_TYPE_UNION_INTERNALLY_TAGGED: {
                    assert(pl->ext.un.tag_map != NULL);
                    assert(pl->ext.un.tag_offset == 0);
                    uint64_t un_tag;
                    if (itype == GSIT_DESERIALIZE) {
                        // peek tag from stream by tag_size
                        if (ROSALIA__SERIALIZATION_DECORATE(ptrdiff)(buf_end, buf) < pl->ext.un.tag_size) {
                            return LS_ERR;
                        }
                        raw_stream rs = rs_init(buf);
                        switch (pl->ext.un.tag_size) {
                            case 1: {
                                un_tag = rs_r_uint8(&rs);
                            } break;
                            case 2: {
                                un_tag = rs_r_uint16(&rs);
                            } break;
                            case 4: {
                                un_tag = rs_r_uint32(&rs);
                            } break;
                            case 8: {
                                un_tag = rs_r_uint64(&rs);
                            } break;
                            default: {
                                assert(0);
                            } break;
                        }
                    } else {
                        switch (pl->ext.un.tag_size) {
                            case 1: {
                                un_tag = (uint64_t)(*(uint8_t*)in_p);
                            } break;
                            case 2: {
                                un_tag = (uint64_t)(*(uint16_t*)in_p);
                            } break;
                            case 4: {
                                un_tag = (uint64_t)(*(uint32_t*)in_p);
                            } break;
                            case 8: {
                                un_tag = (uint64_t)(*(uint64_t*)in_p);
                            } break;
                            default: {
                                assert(0);
                            } break;
                        }
                    }
                    assert(pl->ext.un.tag_max > 0);
                    if (un_tag >= pl->ext.un.tag_max) {
                        return LS_ERR;
                    }
                    size_t csize = ROSALIA__SERIALIZATION_INTERNAL(layout_serializer_impl)(itype, pl->ext.un.tag_map[un_tag], in_p, out_p, cbuf, ebuf);
                    if (csize == LS_ERR) {
                        return LS_ERR;
                    }
                    rsize += csize;
                    if (use_buf == true) {
                        cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, csize);
                    }
                } break;
                case SL_TYPE_UNION_EXTERNALLY_TAGGED: {
                    assert(pl->ext.un.tag_map != NULL);
                    void* tag_p;
                    if (itype == GSIT_DESERIALIZE) {
                        tag_p = ROSALIA__SERIALIZATION_DECORATE(ptradd)(obj_out, pl->ext.un.tag_offset);
                    } else {
                        tag_p = ROSALIA__SERIALIZATION_DECORATE(ptradd)(obj_in, pl->ext.un.tag_offset);
                    }
                    uint64_t un_tag;
                    switch (pl->ext.un.tag_size) {
                        case 1: {
                            un_tag = (uint64_t) * ((uint8_t*)tag_p);
                        } break;
                        case 2: {
                            un_tag = (uint64_t) * ((uint16_t*)tag_p);
                        } break;
                        case 4: {
                            un_tag = (uint64_t) * ((uint32_t*)tag_p);
                        } break;
                        case 8: {
                            un_tag = (uint64_t) * ((uint64_t*)tag_p);
                        } break;
                        default: {
                            assert(0);
                        } break;
                    }
                    assert(pl->ext.un.tag_max > 0);
                    if (un_tag >= pl->ext.un.tag_max) {
                        return LS_ERR;
                    }
                    const serialization_layout* tag_selected_union_layout = pl->ext.un.tag_map[un_tag];
                    if (tag_selected_union_layout != NULL) {
                        size_t csize = ROSALIA__SERIALIZATION_INTERNAL(layout_serializer_impl)(itype, tag_selected_union_layout, in_p, out_p, cbuf, ebuf);
                        if (csize == LS_ERR) {
                            return LS_ERR;
                        }
                        rsize += csize;
                        if (use_buf == true) {
                            cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, csize);
                        }
                    }
                } break;
                case SL_TYPE_COMPLEX: {
                    assert(pl->ext.layout != NULL);
                    size_t csize = ROSALIA__SERIALIZATION_INTERNAL(layout_serializer_impl)(itype, pl->ext.layout, in_p, out_p, cbuf, ebuf);
                    if (csize == LS_ERR) {
                        return LS_ERR;
                    }
                    rsize += csize;
                    if (use_buf == true) {
                        cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, csize);
                    }
                } break;
                case SL_TYPE_CUSTOM: {
                    assert(pl->ext.serializer != NULL);
                    size_t csize = pl->ext.serializer(itype, in_p, out_p, cbuf, ebuf);
                    if (csize == LS_ERR) {
                        return LS_ERR;
                    }
                    rsize += csize;
                    if (use_buf == true) {
                        cbuf = ROSALIA__SERIALIZATION_DECORATE(ptradd)(cbuf, csize);
                    }
                } break;
            }
            if (in_ex == true) {
                in_p = ROSALIA__SERIALIZATION_DECORATE(ptradd)(in_p, typesize);
            }
            if (out_ex == true) {
                out_p = ROSALIA__SERIALIZATION_DECORATE(ptradd)(out_p, typesize);
            }
        }
        if (itype == GSIT_DESTROY && is_ptr == true) {
            free(*(void**)ROSALIA__SERIALIZATION_DECORATE(ptradd)(obj_in, pl->data_offset));
        }
        pl++;
    }
    return rsize;
}

ROSALIA__SERIALIZATION_DEF size_t ROSALIA__SERIALIZATION_DECORATE(layout_serializer)(GSIT itype, const serialization_layout* layout, void* obj_in, void* obj_out, void* buf, void* buf_end)
{
    // deserializing, first zero init the obj, and on copy/deserialization error, destroy it, so we don't leak memory
    if (itype == GSIT_COPY || itype == GSIT_DESERIALIZE) {
        ROSALIA__SERIALIZATION_INTERNAL(layout_serializer_impl)
        (GSIT_INITZERO, layout, obj_out, NULL, buf, buf_end);
    }
    size_t ret = ROSALIA__SERIALIZATION_INTERNAL(layout_serializer_impl)(itype, layout, obj_in, obj_out, buf, buf_end);
    if ((itype == GSIT_COPY || itype == GSIT_DESERIALIZE) && ret == LS_ERR) {
        ROSALIA__SERIALIZATION_INTERNAL(layout_serializer_impl)
        (GSIT_DESTROY, layout, obj_out, NULL, buf, buf_end);
    }
    return ret;
}

#ifdef __cplusplus
}
#endif

#endif
