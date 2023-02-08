#include <stdint.h>
#include <stdio.h>

#define ROSALIA_VECTOR_IMPLEMENTATION
#include "rosalia/vector.h"

#include "tests/tests.h"

void run_test_vector()
{
    uint32_t* v1 = NULL;
    for (int i = 0; i < 10; i++) {
        VEC_PUSH(&v1, i);
    }
    printf("v1 len(%zu) cap(%zu):", VEC_LEN(&v1), VEC_CAP(&v1));
    for (int i = 0; i < VEC_LEN(&v1); i++) {
        printf(" %u", v1[i]);
    }
    printf("\n");

    printf("vector - ok\n");
}
