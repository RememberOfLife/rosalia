#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO unserializable type that throws error on serialize and copy
//TODO api helpers to write easy custom serializer functions if the type is known
//TODO more detailed error reporting?
//TODO maybe max depth for recursive ptr types
/*TODO tagged union serializer type, usage:

    enum TUT {
        TUT_NONE = 0,
        TUT_FOO,
        TUT_BAR,
        TUT_BAZ,
        TUT_COUNT,
        TUT_MAX_SIZE = UINT32_MAX,
    };


    struct tu {
        TUT t; // must be first in base struct and union
        uint32_t bv; // any optional extras in the base type
        uint32_t cv; // any optional extras in the base type
    };
    struct tu_foo {
        tu base;
        char* str;
    };
    struct tu_bar {
        tu base;
        timestamp ts;
    };
    union tu_any {
        tu base;
        tu_foo foo;
        tu_bar bar;
    };
    const serialization_layout* sl_tu_map[TUT_COUNT] = {
        [TUT_NONE] = sl_tu,
        [TUT_FOO] = sl_foo, // includes serializer for base struct (i.e. tag)
        [TUT_BAR] = sl_bar,
        [TUT_BAZ] = sl_tu,
    };
    const serialization_layout sl_tu_any[] = {
        {
            SL_TYPE_INTERNALY_TAGGED_UNION, 0,
            .ext.union.tagtypesize = sizeof(TUT),
            .ext.union.typemap = sla_tu,
        },
        {SL_TYPE_STOP},
    };

    use like:
    struct has_inside_tag {
        // other members
        tu_any union_member;
        // other members
    };
    const serialization_layout sl_hum {
        // other members
        {SL_TYPE_COMPLEX, offsetof(has_inside_tag, union_member), .ext.layout = sl_tu_any},
        // other members
        {SL_TYPE_STOP},
    };


    tag outside of union:
    struct has_outside_tag {
        // other members
        TUT tag;
        union {
            tu_foo foo;
            tu_bar bar;
        } u;
        // other members
    };
    const serialization_layout* sl_tu_map[TUT_COUNT] = {
        [TUT_NONE] = NULL,
        [TUT_FOO] = sl_foo,
        [TUT_BAR] = sl_bar,
        [TUT_BAZ] = NULL,
    }
    const serialization_layout sl_hot[] = {
        {SL_TYPE_U32, offsetof(has_outside_tag, tag)},
        {
            SL_TYPE_EXTERNALLY_TAGGED_UNION,
            offsetof(has_outside_tag, u),
            .ext.union.tagtypesize = sizeof(TUT),
            .ext.union.tagoffset = offsetof(has_outside_tag, tag),
            .ext.union.typemap = sla_tu,
        }
        {SL_TYPE_STOP},
    };

    //TODO what about sparse enums for union tags? later

*/

/////
// utils for serialization

void* ptradd(void* p, size_t v);
void* ptrsub(void* p, size_t v);
size_t ptrdiff(const void* p_end, const void* p_start);

typedef struct blob_s {
    size_t len;
    void* data;
} blob;

void blob_create(blob* b, size_t len);

void blob_resize(blob* b, size_t len, bool preserve_data);

void blob_destroy(blob* b);

/////
// event serialization entries

//NOTE:
// complex types like uint32_t** ptr_to_ptrarray; require a separate layout for the uint32_t* serialization of the uint32_t* elems in the ptrarray

typedef enum __attribute__((__packed__)) SL_TYPE_E {
    SL_TYPE_NULL = 0,

    SL_TYPE_BOOL,
    SL_TYPE_U8,
    SL_TYPE_U32,
    SL_TYPE_U64,
    SL_TYPE_SIZE,
    //TODO more primitives

    // pseudo primitives
    SL_TYPE_STRING, // char* member;
    SL_TYPE_BLOB, // blob member;

    // extension types, must supply typesize
    // SL_TYPE_UNION, //TODO
    SL_TYPE_COMPLEX, // use ext.layout to specify the type //TODO rename to struct?
    SL_TYPE_CUSTOM, // use ext.serializer to supply the serializer to use

    SL_TYPE_STOP, // signals the end of serialization items in this sl

    SL_TYPE_COUNT,

    SL_TYPE_TYPEMASK = 0b00111111,
    // these are flags applicable to all serializable types
    SL_TYPE_PTR = 0b01000000,
    SL_TYPE_ARRAY = 0b10000000, // if ptr: use len.offset else: len.immediate, to get size of the array, len.offset always locates a size_t
    SL_TYPE_PTRARRAY = SL_TYPE_PTR | SL_TYPE_ARRAY, // convenience type for sl definitions

    SL_TYPE_SIZE_MAX = UINT8_MAX,
} SL_TYPE;

// general serializer invocation type
typedef enum __attribute__((__packed__)) GSIT_E {
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

struct serialization_layout_s {
    SL_TYPE type;
    size_t data_offset;

    union {
        const serialization_layout* layout;
        // size_t serializer(GSIT itype, void* obj_in, void* obj_out, void* buf, void* buf_end)
        custom_serializer_t* serializer;
    } ext;

    size_t typesize; // req'd only if PTR || ARRAY

    union {
        size_t immediate;
        size_t offset;
        size_t max; // optional, only for ptrarray, ignored if 0
    } len; // req'd only if PTR || ARRAY
};

static const size_t LS_ERR = SIZE_MAX;

// returned size_t is only valid if itype SIZE/SERIALIZE/DESERIALIZE
// if return value is LS_ERR then an error occured, can not happen during init or destroy
// deserialization errors are automatically cleaned up and do not leak memory (assuming all used custom functions do this as well)
size_t layout_serializer(GSIT itype, const serialization_layout* layout, void* obj_in, void* obj_out, void* buf, void* buf_end);

//TODO rather give lookup array?
// primitive serializers
custom_serializer_t ls_primitive_bool_serializer;
custom_serializer_t ls_primitive_u8_serializer;
custom_serializer_t ls_primitive_u32_serializer;
custom_serializer_t ls_primitive_u64_serializer;
custom_serializer_t ls_primitive_size_serializer;
custom_serializer_t ls_primitive_string_serializer;
custom_serializer_t ls_primitive_blob_serializer;

#ifdef __cplusplus
}
#endif
