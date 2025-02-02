#include <stdint.h>
#include <stdio.h>

#define ROSALIA_VECTOR_IMPLEMENTATION
#include "rosalia/vector.h"

#include "tests/tests.h"

void run_test_vector()
{
    VECTOR(uint32_t) v1;
    VEC_CREATE(&v1, 0);
    for (int i = 0; i < 10; i++) {
        VEC_PUSH(&v1, i);
    }
    assert(VEC_LEN(&v1) == 10);
    assert(VEC_CAP(&v1) >= 10);
    for (int i = 0; i < VEC_LEN(&v1); i++) {
        assert(v1[i] == i);
    }
    VEC_DESTROY(&v1);
    assert(v1 == NULL);

    printf(". vector\n");
}
