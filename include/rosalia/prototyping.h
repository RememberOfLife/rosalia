#ifndef ROSALIA_PROTOTYPING_H_INCLUDE
#define ROSALIA_PROTOTYPING_H_INCLUDE

#ifdef ROSALIA_PROTOTYPING_STATIC
#define ROSALIA__PROTOTYPING_DEC static
#define ROSALIA__PROTOTYPING_DEC_EXT static
#define ROSALIA__PROTOTYPING_DEF static
#else
#define ROSALIA__PROTOTYPING_DEC
#define ROSALIA__PROTOTYPING_DEC_EXT extern
#define ROSALIA__PROTOTYPING_DEF
#endif

#define ROSALIA_PROTOTYPING_VERSION_MAJOR 0
#define ROSALIA_PROTOTYPING_VERSION_MINOR 1
#define ROSALIA_PROTOTYPING_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

#include "rosalia/util.h"

// #define VEC(type) type*

/*
problems:
- a function that takes a container as arg, may not modify it, otherwise the pointer gets lost
    - it would instead take a container*, i.e. int**, this seems fine I guess?
*/

// proper ownership
// #define CONTAINER_TYPE(user_type)     \
//     struct {                          \
//         user_type contained;          \
//         ZST __rosalia_container_type; \
//     }
// then while using, check that __rosalia_container_type is at the expected offsetof where it should be, AFTER the user type
// but its actualy zero sized so no extra cost and we can check if the thing we are given is actually an expected container

// easy iterators for types, the type is the container type!!, somehow make sure to check that it IS a container

// or objectify containers when you want it
// w.r.t. passing around, this could make sure we are always passing and using a properly indirected version of our container

// one thing to mark it from a normal container, such that you know if its indirected or not
// and another union hack thing so you can access both members at comp time, but will always only access one, based on the comparison of where the container marker is, e.g. front or back, maybe could make another marker? i.e. two marker flags, one for container another for stable obj?

// can use similar compile time flag feature for a general purpose thing!
// or just to mark: this container is sorted or no, this container needs str or diy hashing or no, this container has allocator or no

/*
check zig for cool containers!

ARRAYLIST (vector)
INDIRECT_ARRAYLIST (dqueue)
SORTED_ARRAYLIST (sorted vector)
SORTED_INDIRECT_ARRAYLIST (sorted dqueue)

HASH_MAP
TREE_MAP
both available as multi, these are implicitly the types for ordered and unordered, use zero sized value for a set

SLINKLIST
DLINKLIST

REF_CELL

ITERATOR

SPAN

STREAM

*/

typedef struct {
} _ROSA_ZST;

// obj is either by val or by ref
#define OBJ_CONTAINER_SMF_OBJ_VAL_REF obj_val_ref
#define OBJ_CONTAINER_SPEC_VAL_PRE _ROSA_ZST _rosalia_smf_##OBJ_CONTAINER_SMF_OBJ_VAL_REF
#define OBJ_CONTAINER_SPEC_VAL_POST
#define OBJ_CONTAINER_SPEC_REF_PRE
#define OBJ_CONTAINER_SPEC_REF_POST _ROSA_ZST _rosalia_smf_##OBJ_CONTAINER_SMF_OBJ_VAL_REF

// obj has order relation defined (ORD) or no order relation (NOR)
#define OBJ_CONTAINER_SMF_OBJ_NOR_ORD obj_nor_ord
#define OBJ_CONTAINER_SPEC_NOR_PRE _ROSA_ZST _rosalia_smf_##OBJ_CONTAINER_SMF_OBJ_NOR_ORD
#define OBJ_CONTAINER_SPEC_NOR_POST
#define OBJ_CONTAINER_SPEC_ORD_PRE
#define OBJ_CONTAINER_SPEC_ORD_POST _ROSA_ZST _rosalia_smf_##OBJ_CONTAINER_SMF_OBJ_NOR_ORD

#define CATI(x, y) x##y
#define MCAT(x, y) CATI(x, y)

// use this to do struct description disambiguation if two files use the exactly same thing in the same counter order at the same line
// should be rather unlikely but you can disambiguate your file like this if you want
#define STRUCT_DESC_DISAMBIGUATION

#define OBJ_CONTAINER(OBJ_CONTAINER_SPEC, OBJ_CONTAINER_ORDER, obj_type, struct_desc)         \
    struct MCAT(MCAT(MCAT(struct_desc, __COUNTER__), __LINE__), STRUCT_DESC_DISAMBIGUATION) { \
        _ROSA_ZST _rosalia_sm_container;                                                      \
        OBJ_CONTAINER_SPEC_##OBJ_CONTAINER_SPEC##_PRE;                                        \
        OBJ_CONTAINER_SPEC_##OBJ_CONTAINER_ORDER##_PRE;                                       \
        union {                                                                               \
            obj_type* obj_p;                                                                  \
            obj_type obj;                                                                     \
        } u;                                                                                  \
        OBJ_CONTAINER_SPEC_##OBJ_CONTAINER_SPEC##_POST;                                       \
        OBJ_CONTAINER_SPEC_##OBJ_CONTAINER_ORDER##_POST;                                      \
    }

#define PROTO_ARRAYLIST(OBJ_CONTAINER_SPEC, OBJ_CONTAINER_ORDER, val_type)        \
    OBJ_CONTAINER(                                                                \
        OBJ_CONTAINER_SPEC,                                                       \
        OBJ_CONTAINER_ORDER,                                                      \
        val_type*,                                                                \
        arraylist_##OBJ_CONTAINER_SPEC##_##OBJ_CONTAINER_ORDER##_##val_type##__IC \
    )

// #define PROTO_HASHMAP(OBJ_CONTAINER_SPEC, OBJ_CONTAINER_ORDER, key_type, val_type) \
//     OBJ_CONTAINER(                                                                 \
//         OBJ_CONTAINER_SPEC,                                                        \
//         OBJ_CONTAINER_ORDER,                                                       \
//         struct {                                                                   \
//             key_type key;                                                          \
//             val_type val;                                                          \
//         }                                                                          \
//     )

// #define PROTO_HASHSET(OBJ_CONTAINER_SPEC, OBJ_CONTAINER_ORDER, key_type) \
//     PROTO_HASHMAP(OBJ_CONTAINER_SPEC, OBJ_CONTAINER_ORDER, key_type, _ROSA_ZST)

// #define PROTO_TREEMAP(OBJ_CONTAINER_SPEC, OBJ_CONTAINER_ORDER, key_type, val_type) \
//     OBJ_CONTAINER(                                                                 \
//         OBJ_CONTAINER_SPEC,                                                        \
//         OBJ_CONTAINER_ORDER,                                                       \
//         struct {                                                                   \
//             key_type key;                                                          \
//             val_type val;                                                          \
//         }                                                                          \
//     )

// #define PROTO_TREESET(OBJ_CONTAINER_SPEC, OBJ_CONTAINER_ORDER, key_type) \
//     PROTO_TREEMAP(OBJ_CONTAINER_SPEC, OBJ_CONTAINER_ORDER, key_type, _ROSA_ZST)

// #define ITERATOR(container_type)        \
//     struct {                            \
//         _ROSA_ZST _rosalia_sm_iterator; \
//         container_type container;       \
//         size_t position;                \
//     }

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_PROTOTYPING_IMPLEMENTATION) && !defined(ROSALIA_PROTOTYPING_H_IMPL)
#define ROSALIA_PROTOTYPING_H_IMPL

#endif
