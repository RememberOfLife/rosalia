#pragma once

#include <stdint.h>

#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

const static rosa_semver rosa_rand_version = (rosa_semver){0, 2, 0};

// using PCG32 minimal seeded via splitmix64
typedef struct rosa_fast_prng_t {
    uint64_t state;
    uint64_t inc;
} rosa_fast_prng;

void rosa_fprng_srand(rosa_fast_prng* fprng, uint64_t seed);
uint32_t rosa_fprng_rand(rosa_fast_prng* fprng);

#ifdef __cplusplus
}
#endif

//TODO serialization.h compatibility and fast_prng serialization layout
