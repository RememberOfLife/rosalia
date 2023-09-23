#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROSALIA_SERIALIZATION_IMPLEMENTATION
#include "rosalia/serialization.h"

#include "tests/tests.h"

///////
// basic complex type testing

typedef struct complex_sub_s {
    uint64_t a;
    bool b;
} complex_sub;

const serialization_layout sl_cs[] = {
    {SL_TYPE_U64, offsetof(complex_sub, a)},
    {SL_TYPE_BOOL, offsetof(complex_sub, b)},
    {SL_TYPE_STOP},
};

///////
// for externally tagged union testing

typedef enum MYTAG_E {
    MYTAG_NONE = 0,
    MYTAG_NO_MEMBER,
    MYTAG_CS,
    MYTAG_SIZE,
    MYTAG_COUNT,
    MYTAG_MAX_SIZE = UINT32_MAX,
} MYTAG;

typedef struct has_union_text_s {
    uint32_t x;
    MYTAG tag;

    union {
        complex_sub cs;
        size_t s;
    } un;
} has_union_text;

//TODO this probably needs some kind of shorthand syntax
const serialization_layout sl_s[] = {
    {SL_TYPE_SIZE, 0},
    {SL_TYPE_STOP},
};

const serialization_layout* sl_hut_un_map[MYTAG_COUNT] = {
    [MYTAG_NONE] = NULL,
    [MYTAG_NO_MEMBER] = NULL,
    [MYTAG_CS] = sl_cs,
    [MYTAG_SIZE] = sl_s,
};

const serialization_layout sl_hut[] = {
    {SL_TYPE_U32, offsetof(has_union_text, x)},
    {SL_TYPE_UAUTOP2(has_union_text, tag), offsetof(has_union_text, tag)},
    {
        SL_TYPE_UNION_EXTERNALLY_TAGGED,
        offsetof(has_union_text, un),
        .ext.un.tag_size = sizeof(MYTAG),
        .ext.un.tag_offset = offsetof(has_union_text, tag),
        .ext.un.tag_max = MYTAG_COUNT,
        .ext.un.tag_map = sl_hut_un_map,
    },
    {SL_TYPE_STOP},
};

///////
// for internally tagged union testing

typedef enum IUTAG_E {
    IUTAG_NONE = 0,
    IUTAG_FOO,
    IUTAG_BAR,
    IUTAG_BAZ,
    IUTAG_COUNT,
    IUTAG_MAX_SIZE = UINT32_MAX,
} IUTAG;

typedef struct tu_s {
    IUTAG t;
    uint32_t b;
} tu;

const serialization_layout sl_tu[] = {
    {SL_TYPE_UAUTOP2(tu, t), offsetof(tu, t)},
    {SL_TYPE_U32, offsetof(tu, b)},
    {SL_TYPE_STOP},
};

typedef struct tu_foo_s {
    tu base;
    uint32_t x;
} tu_foo;

const serialization_layout sl_tu_foo[] = {
    {SL_TYPE_COMPLEX, offsetof(tu_foo, base), .ext.layout = sl_tu},
    {SL_TYPE_U32, offsetof(tu_foo, x)},
    {SL_TYPE_STOP},
};

typedef struct tu_bar_s {
    tu base;
    uint64_t y;
} tu_bar;

const serialization_layout sl_tu_bar[] = {
    {SL_TYPE_COMPLEX, offsetof(tu_bar, base), .ext.layout = sl_tu},
    {SL_TYPE_U64, offsetof(tu_bar, y)},
    {SL_TYPE_STOP},
};

typedef union tu_any_u {
    tu base;
    tu_foo foo;
    tu_bar bar;
} tu_any;

const serialization_layout* sl_tu_map[IUTAG_COUNT] = {
    [IUTAG_NONE] = sl_tu,
    [IUTAG_FOO] = sl_tu_foo,
    [IUTAG_BAR] = sl_tu_bar,
    [IUTAG_BAZ] = sl_tu,
};

const serialization_layout sl_tu_any[] = {
    {
        SL_TYPE_UNION_INTERNALLY_TAGGED,
        .ext.un.tag_size = sizeof(IUTAG),
        .ext.un.tag_max = IUTAG_COUNT,
        .ext.un.tag_map = sl_tu_map,
    },
    {SL_TYPE_STOP},
};

///////
// strings testing

typedef struct complex_with_string_s {
    char* str;
} complex_with_string;

const serialization_layout sl_cws[] = {
    {SL_TYPE_STRING, offsetof(complex_with_string, str)},
    {SL_TYPE_STOP},
};

///////
// strings ptrarray testing

typedef struct complex_with_string_ptrarray_s {
    uint64_t str_c;
    char** str;
} complex_with_string_ptrarray;

const serialization_layout sl_cwspa[] = {
    {SL_TYPE_U64, offsetof(complex_with_string_ptrarray, str_c)},
    {SL_TYPE_STRING | SL_TYPE_PTRARRAY, offsetof(complex_with_string_ptrarray, str), .len.offset = offsetof(complex_with_string_ptrarray, str_c)},
    {SL_TYPE_STOP},
};

//TODO test normal just ptr
//TODO test normal just array
//TODO test normal ptr array

///////
// testing helpers

void print_buffer(uint8_t* buf, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");
}

void check_deserialize_equal(void* obj1, void* obj2, const serialization_layout* layout)
{
    size_t ls_ret;
    size_t buf_size;
    void* buf1;
    void* buf2;
    // get req'd buffer size
    buf_size = layout_serializer(GSIT_SIZE, layout, obj1, NULL, NULL, NULL);
    assert(buf_size != LS_ERR);
    buf1 = malloc(buf_size);
    buf2 = malloc(buf_size);
    // serialize obj1 into buffer1
    ls_ret = layout_serializer(GSIT_SERIALIZE, layout, obj1, NULL, buf1, NULL);
    assert(ls_ret != LS_ERR);
    // deserialize from buffer into other obj2
    ls_ret = layout_serializer(GSIT_DESERIALIZE, layout, NULL, obj2, buf1, ptradd(buf1, buf_size));
    assert(ls_ret != LS_ERR);
    // reserialize obj2 into other buffer2
    ls_ret = layout_serializer(GSIT_SERIALIZE, layout, obj2, NULL, buf2, NULL);
    assert(ls_ret != LS_ERR);
    // compare buffer1 and buffer2
    assert(memcmp(buf1, buf2, buf_size) == 0);
    // print_buffer(buf1, buf_size);
    free(buf1);
    free(buf2);
}

void check_copy_equal(void* obj1, void* obj2, const serialization_layout* layout)
{
    size_t ls_ret;
    size_t buf_size;
    void* buf1;
    void* buf2;
    // get req'd buffer size
    buf_size = layout_serializer(GSIT_SIZE, layout, obj1, NULL, NULL, NULL);
    assert(buf_size != LS_ERR);
    buf1 = malloc(buf_size);
    buf2 = malloc(buf_size);
    // copy obj1 into obj2
    ls_ret = layout_serializer(GSIT_COPY, layout, obj1, obj2, NULL, NULL);
    assert(ls_ret != LS_ERR);
    // serialize obj1 into buffer1
    ls_ret = layout_serializer(GSIT_SERIALIZE, layout, obj1, NULL, buf1, NULL);
    assert(ls_ret != LS_ERR);
    // serialize obj2 into buffer2
    ls_ret = layout_serializer(GSIT_SERIALIZE, layout, obj2, NULL, buf2, NULL);
    assert(ls_ret != LS_ERR);
    // compare buffer1 and buffer2
    assert(memcmp(buf1, buf2, buf_size) == 0);
    // print_buffer(buf1, buf_size);
    free(buf1);
    free(buf2);
}

void check_destroy(void* obj, const serialization_layout* layout)
{
    size_t ls_ret;
    ls_ret = layout_serializer(GSIT_DESTROY, layout, obj, NULL, NULL, NULL);
    assert(ls_ret != LS_ERR);
}

///////
// testing

void run_test_serialization()
{

    // TODO
    // raw_stream (serialize random types and check that deserialize produces expected output)

    // test simple struct
    complex_sub t3_o1 = (complex_sub){
        .a = 0x1122334455667788,
        .b = false,
    };
    complex_sub t3_o2;
    complex_sub t3_o3;
    check_deserialize_equal(&t3_o1, &t3_o2, sl_cs);
    check_copy_equal(&t3_o2, &t3_o3, sl_cs);

    check_destroy(&t3_o1, sl_cs);
    check_destroy(&t3_o2, sl_cs);
    check_destroy(&t3_o3, sl_cs);

    // test external union
    has_union_text t1_o1 = (has_union_text){
        .x = 0x11223344,
        .tag = MYTAG_CS,
        .un.cs = {
            .a = 0xAABBCCDDAABBCCDD,
            .b = false,
        },
    };
    has_union_text t1_o2;
    has_union_text t1_o3;
    check_deserialize_equal(&t1_o1, &t1_o2, sl_hut);
    check_copy_equal(&t1_o2, &t1_o3, sl_hut);
    t1_o1.tag = MYTAG_NO_MEMBER;
    check_deserialize_equal(&t1_o1, &t1_o2, sl_hut);
    check_copy_equal(&t1_o2, &t1_o3, sl_hut);

    check_destroy(&t1_o1, sl_hut);
    check_destroy(&t1_o2, sl_hut);
    check_destroy(&t1_o3, sl_hut);

    // test internal union
    tu_any t2_o1 = (tu_any){
        .foo = {
            .base = {
                .t = IUTAG_FOO,
                .b = 0x11223344,
            },
            .x = 0x88776655,
        },
    };
    tu_any t2_o2;
    tu_any t2_o3;
    check_deserialize_equal(&t2_o1, &t2_o2, sl_tu_any);
    check_copy_equal(&t2_o2, &t2_o3, sl_tu_any);
    t2_o1.base.t = IUTAG_BAZ;
    check_deserialize_equal(&t2_o1, &t2_o2, sl_tu_any);
    check_copy_equal(&t2_o2, &t2_o3, sl_tu_any);

    check_destroy(&t2_o1, sl_tu_any);
    check_destroy(&t2_o2, sl_tu_any);
    check_destroy(&t2_o3, sl_tu_any);

    // test string
    complex_with_string t4_o1 = (complex_with_string){
        .str = strdup("abc"),
    };
    complex_with_string t4_o2;
    complex_with_string t4_o3;
    check_deserialize_equal(&t4_o1, &t4_o2, sl_cws);
    check_copy_equal(&t4_o2, &t4_o3, sl_cws);

    check_destroy(&t4_o1, sl_cws);
    check_destroy(&t4_o2, sl_cws);
    check_destroy(&t4_o3, sl_cws);

    // test string ptrarray
    complex_with_string_ptrarray t5_o1;
    t5_o1.str_c = 2;
    if (t5_o1.str_c == 0) {
        t5_o1.str = NULL;
    } else {
        t5_o1.str = malloc(sizeof(char*) * t5_o1.str_c);
    }
    for (size_t i = 0; i < t5_o1.str_c; i++) {
        size_t assign_len = 4;
        t5_o1.str[i] = malloc(sizeof(char) * (assign_len + 1));
        for (size_t j = 0; j < assign_len; j++) {
            t5_o1.str[i][j] = 'a' + i; // look for HEX 61 + j in the debug print output
        }
        t5_o1.str[i][assign_len] = '\0';
    }
    complex_with_string_ptrarray t5_o2;
    complex_with_string_ptrarray t5_o3;
    check_deserialize_equal(&t5_o1, &t5_o2, sl_cwspa);
    check_copy_equal(&t5_o2, &t5_o3, sl_cwspa);

    check_destroy(&t5_o1, sl_cwspa);
    check_destroy(&t5_o2, sl_cwspa);
    check_destroy(&t5_o3, sl_cwspa);

    //TODO test blob, arr, ptr, ptrarr

    printf(". serialization\n");
}
