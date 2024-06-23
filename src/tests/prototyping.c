#include "rosalia/types.h"

#define ROSALIA_PROTOTYPING_IMPLEMENTATION
#include "rosalia/prototyping.h"

typedef struct my_struc_s {
    PROTO_ARRAYLIST(VAL, NOR, u32) arrlist_one;
    PROTO_ARRAYLIST(VAL, NOR, u32) arrlist_two;
    PROTO_HASHMAP(REF, NOR, u32, u32) hash_map;
    PROTO_HASHSET(REF, ORD, u32) hash_set;
    PROTO_TREEMAP(VAL, NOR, u32, u32) prio_tm;
    PROTO_TREESET(VAL, ORD, u32) prio_set;
} my_struc;

void obj_passing_test(PROTO_ARRAYLIST(VAL, NOR, u32)* a)
{
}

void run_test_prototyping()
{
    my_struc a;
    obj_passing_test((void*)&a.arrlist_one);
}
