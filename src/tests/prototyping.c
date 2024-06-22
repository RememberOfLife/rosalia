#include "rosalia/types.h"

#define ROSALIA_PROTOTYPING_IMPLEMENTATION
#include "rosalia/prototyping.h"

typedef struct my_struc_s {
    PROTO_ARRAYLIST(VAL, NOR, u32) arrlist_one; // rosa arraylist
    PROTO_ARRAYLIST(VAL, NOR, u32) arrlist_two; // rosa arraylist
    // PROTO_HASHSET(REF, ORD, u32) hash_set;
    // PROTO_TREEMAP(VAL, NOR, u32, u32) prio_tree;
    // PROTO_TREESET(VAL, ORD, u32) prio_tree;
} my_struc;

void run_test_prototyping()
{
}
