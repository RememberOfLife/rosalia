#include <stdint.h>
#include <stdio.h>

#define ROSALIA_RAND_IMPLEMENTATION
#include "rosalia/rand.h"

#include "tests/tests.h"

void run_test_rand()
{
    fast_prng rng;
    fprng_srand(&rng, 42);
    uint64_t acc;
    for (int i = 0; i < 100; i++) {
        acc += fprng_rand(&rng);
    }
    printf("rand - ok\n");
}
