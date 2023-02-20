#ifndef ROSALIA_RAND_H_INCLUDE
#define ROSALIA_RAND_H_INCLUDE

#include <stdint.h>

#ifdef ROSALIA_RAND_STATIC
#define ROSALIA__RAND_DEC static
#define ROSALIA__RAND_DEC_EXT static
#define ROSALIA__RAND_DEF static
#else
#define ROSALIA__RAND_DEC
#define ROSALIA__RAND_DEC_EXT extern
#define ROSALIA__RAND_DEF
#endif

#ifdef ROSALIA_RAND_DECORATE
#define ROSALIA__RAND_DECORATE(ident) ROSALIA_RAND_DECORATE(ident)
#else
#define ROSALIA__RAND_DECORATE(ident) ident
#endif

#define ROSALIA_RAND_VERSION_MAJOR 0
#define ROSALIA_RAND_VERSION_MINOR 1
#define ROSALIA_RAND_VERSION_PATCH 4

#ifdef __cplusplus
extern "C" {
#endif

//TODO decorate struct
// using PCG32 minimal seeded via splitmix64
typedef struct fast_prng_t {
    uint64_t state;
    uint64_t inc;
} fast_prng;

ROSALIA__RAND_DEC void ROSALIA__RAND_DECORATE(fprng_srand)(fast_prng* fprng, uint64_t seed);
ROSALIA__RAND_DEC uint32_t ROSALIA__RAND_DECORATE(fprng_rand)(fast_prng* fprng);

#ifdef __cplusplus
}
#endif

#endif

//TODO serialization.h compatibility and fast_prng serialization layout

#if defined(ROSALIA_RAND_IMPLEMENTATION) && !defined(ROSALIA_RAND_H_IMPL)
#define ROSALIA_RAND_H_IMPL

#define ROSALIA__RAND_INTERNAL(ident) rosalia__rand_internal_##ident

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__RAND_DEF void ROSALIA__RAND_DECORATE(fprng_srand)(fast_prng* fprng, uint64_t seed)
{
    // splitmix pre-seed
    uint64_t z = (seed + UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    fprng->state = z ^ (z >> 31);
    fprng->inc = 1;
}

ROSALIA__RAND_DEF uint32_t ROSALIA__RAND_DECORATE(fprng_rand)(fast_prng* fprng)
{
    uint64_t oldstate = fprng->state;
    // Advance internal state
    fprng->state = oldstate * 6364136223846793005ULL + (fprng->inc | 1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

#ifdef __cplusplus
}
#endif

#endif
