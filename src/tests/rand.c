#include <stdint.h>
#include <stdio.h>

#include "rosalia/rand.h"
#include "rosalia/test.h"
#include "rosalia/util.h"

#include "tests.h"

ROSA_TEST(rand)
{
    ROSA_TEST_CASE("general functionality")
    {
        rosa_fast_prng rng;
        rosa_fprng_srand(&rng, 42);
        uint64_t acc;
        for (int i = 0; i < 100; i++) {
            acc += rosa_fprng_rand(&rng);
        }
    }
    //TODO seeding gives same results
    //TODO re-seeding works and gives same results
}
