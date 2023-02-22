#include <stdint.h>
#include <stdio.h>

#define ROSALIA_JSON_IMPLEMENTATION
#include "rosalia/json.h"

#include "tests/tests.h"

void run_test_json()
{
    //TODO proper testing, this is just the mirabel test initialization
    cj_ovac* root = cj_create_object(0);
    cj_object_append(root, "abc", cj_create_vnull());
    cj_object_append(root, "123", cj_create_f32(7.25));
    cj_ovac* a1 = cj_create_object(0);
    cj_object_append(a1, "a_key", cj_create_vnull());
    cj_object_append(a1, "anotherone", cj_create_str(200, "some string"));
    cj_object_append(root, "obj_key", a1);
    cj_ovac* a2 = cj_create_array(0);
    cj_array_append(a2, cj_create_u64(1));
    cj_array_append(a2, cj_create_u64(3));
    cj_array_append(a2, cj_create_u64(2));
    cj_array_append(a2, cj_create_u64(4));
    cj_object_append(root, "arr_key", a2);
    cj_object_append(root, "17", cj_create_u64(42));
    cj_object_append(root, "mybool", cj_create_bool(false));
    cj_ovac_destroy(root);

    printf(". json\n");
}
