#ifndef ROSALIA_NOISE_H_INCLUDE
#define ROSALIA_NOISE_H_INCLUDE

#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_NOISE_STATIC
#define ROSALIA__NOISE_DEC static
#define ROSALIA__NOISE_DEC_EXT static
#define ROSALIA__NOISE_DEF static
#else
#define ROSALIA__NOISE_DEC
#define ROSALIA__NOISE_DEC_EXT extern
#define ROSALIA__NOISE_DEF
#endif

#define ROSALIA_NOISE_VERSION_MAJOR 0
#define ROSALIA_NOISE_VERSION_MINOR 1
#define ROSALIA_NOISE_VERSION_PATCH 5

#ifdef __cplusplus
extern "C" {
#endif

/*
SquirrelNoise5 - Squirrel's Raw Noise utilities (version 5)

This code is made available under the Creative Commons attribution 3.0 license (CC-BY-3.0 US):
    Attribution in source code comments (even closed-source/commercial code) is sufficient.
    License summary and text available at: https://creativecommons.org/licenses/by/3.0/us/

These noise functions were written by Squirrel Eiserloh as a cheap and simple substitute for
    the [sometimes awful] bit-noise sample code functions commonly found on the web, many of which
    are hugely biased or terribly patterned, e.g. having bits which are on (or off) 75% or even
    100% of the time (or are excessively overkill/slow for our needs, such as MD5 or SHA).

Note: This is work in progress; not all functions have been tested.  Use at your own risk.
    Please report any bugs, issues, or bothersome cases to SquirrelEiserloh at gmail.com.

The following functions are all based on a simple bit-noise hash function which returns an
    unsigned integer containing 32 reasonably-well-scrambled bits, based on a given (signed)
    integer input parameter (position/index) and [optional] seed.  Kind of like looking up a
    value in an infinitely large [non-existent] table of previously rolled random numbers.

These functions are deterministic and random-access / order-independent (i.e. state-free),
    so they are particularly well-suited for use in smoothed/fractal/simplex/Perlin noise
    functions and out-of-order (or or-demand) procedural content generation (i.e. that mountain
    village is the same whether you generated it first or last, ahead of time or just now).

The N-dimensional variations simply hash their multidimensional coordinates down to a single
    32-bit index and then proceed as usual, so while results are not unique they should
    (hopefully) not seem locally predictable or repetitive.

-----------------------------------------------------------------------------------------------
Fast hash of an int32 into a different (unrecognizable) uint32.

Returns an unsigned integer containing 32 reasonably-well-scrambled bits, based on the hash
    of a given (signed) integer input parameter (position/index) and [optional] seed.  Kind of
    like looking up a value in an infinitely large table of previously generated random numbers.

I call this particular approach SquirrelNoise5 (5th iteration of my 1D raw noise function).

Many thanks to Peter Schmidt-Nielsen whose outstanding analysis helped identify a weakness
    in the SquirrelNoise3 code I originally used in my GDC 2017 talk, "Noise-based RNG".
    Version 5 avoids a noise repetition found in version 3 at extremely high position values
    caused by a lack of influence by some of the high input bits onto some of the low output bits.

The revised SquirrelNoise5 function ensures all input bits affect all output bits, and to
    (for me) a statistically acceptable degree.  I believe the worst-case here is in the amount
    of influence input position bit #30 has on output noise bit #0 (49.99%, vs. 50% ideal).
*/

// fast hash of an int32 into a different (unrecognizable) uint32
ROSALIA__NOISE_DEC uint32_t squirrelnoise5(int32_t positionX, uint32_t seed);

// raw pseudorandom noise functions (random-access / deterministic), basis of all other noise
ROSALIA__NOISE_DEC uint32_t get_1d_u32(int32_t index, uint32_t seed);
ROSALIA__NOISE_DEC uint32_t get_2d_u32(int32_t indexX, int32_t indexY, uint32_t seed);
ROSALIA__NOISE_DEC uint32_t get_3d_u32(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed);
ROSALIA__NOISE_DEC uint32_t get_4d_u32(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed);

// same nois, mapped to floats in [0,1] for convenience
ROSALIA__NOISE_DEC float get_1d_zto(int32_t index, uint32_t seed);
ROSALIA__NOISE_DEC float get_2d_zto(int32_t indexX, int32_t indexY, uint32_t seed);
ROSALIA__NOISE_DEC float get_3d_zto(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed);
ROSALIA__NOISE_DEC float get_4d_zto(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed);

// same nois, mapped to floats in [-1,1] for convenience
ROSALIA__NOISE_DEC float get_1d_noto(int32_t index, uint32_t seed);
ROSALIA__NOISE_DEC float get_2d_noto(int32_t indexX, int32_t indexY, uint32_t seed);
ROSALIA__NOISE_DEC float get_3d_noto(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed);
ROSALIA__NOISE_DEC float get_4d_noto(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed);

// end of squirrelnoise licensed code, following are additions

// returns a uint32 in range [0,max_n) i.e. max_n is NOT included in the possible results
// this function eliminates modulo bias
ROSALIA__NOISE_DEC uint32_t noise_get_uintn(int32_t position, uint32_t seed, uint32_t max_n);

// pass str_end NULL as default to use normal null character termination
// str_end points to the first character NOT included in the hash
ROSALIA__NOISE_DEC uint32_t strhash(const char* str, const char* str_end);

//TODO fix coherent noise using proper simplex noise
// typedef struct coherent_noise_s {
//     uint32_t seed;
//     float scale; // what distance to view the noisemap from, smaller is closer (i.e. structures get bigger)
//     uint8_t octaves; // levels of detail noise should have, higher gives more possible detail (finer grain)
//     float lacunarity; // how much detail is added or removed each octave (adjusts frequency), higher gives less blending of octaves
//     float persistence; // how much each octave contributes to the overall shape (adjusts amplitude), higher makes rougher
// } coherent_noise;

// // returns in [-1,1]
// ROSALIA__NOISE_DEC  float coherent_noise_2d(coherent_noise* cn, float x, float y);

//TODO get_2d_noto double and coherent noise double

/*
diy inserts:
emulate rng:
Roll u32
Roll u16
Roll u8
Roll int less than
Roll int in range [a,b] inclusive
Roll float zero to one
Roll float in range [a,b] inclusive
Roll chance (prob of true) -> bool
Roll normal distributed float (i.e. only distribute between -1 and 1)
*/

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_NOISE_IMPLEMENTATION) && !defined(ROSALIA_NOISE_H_IMPL)
#define ROSALIA_NOISE_H_IMPL

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__NOISE_DEF uint32_t squirrelnoise5(int32_t positionX, uint32_t seed)
{
    const uint32_t SQ7_BIT_NOISE1 = 0xd2a80a3f; // 11010010101010000000101000111111
    const uint32_t SQ5_BIT_NOISE2 = 0xa884f197; // 10101000100001001111000110010111
    const uint32_t SQ5_BIT_NOISE3 = 0x6C736F4B; // 01101100011100110110111101001011
    const uint32_t SQ5_BIT_NOISE4 = 0xB79F3ABB; // 10110111100111110011101010111011
    const uint32_t SQ5_BIT_NOISE5 = 0x1b56c4f5; // 00011011010101101100010011110101
    uint32_t mangledBits = (uint32_t)positionX;
    mangledBits *= SQ7_BIT_NOISE1;
    mangledBits += seed;
    mangledBits ^= (mangledBits >> 9);
    mangledBits += SQ5_BIT_NOISE2;
    mangledBits ^= (mangledBits >> 11);
    mangledBits *= SQ5_BIT_NOISE3;
    mangledBits ^= (mangledBits >> 13);
    mangledBits += SQ5_BIT_NOISE4;
    mangledBits ^= (mangledBits >> 15);
    mangledBits *= SQ5_BIT_NOISE5;
    mangledBits ^= (mangledBits >> 17);
    return mangledBits;
}

ROSALIA__NOISE_DEF uint32_t get_1d_u32(int32_t positionX, uint32_t seed)
{
    return squirrelnoise5(positionX, seed);
}

ROSALIA__NOISE_DEF uint32_t get_2d_u32(int32_t indexX, int32_t indexY, uint32_t seed)
{
    const int32_t PRIME_NUMBER = 198491317; // Large prime number with non-boring bits
    return squirrelnoise5(indexX + (PRIME_NUMBER * indexY), seed);
}

ROSALIA__NOISE_DEF uint32_t get_3d_u32(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed)
{
    const int32_t PRIME1 = 198491317; // Large prime number with non-boring bits
    const int32_t PRIME2 = 6542989; // Large prime number with distinct and non-boring bits
    return squirrelnoise5(indexX + (PRIME1 * indexY) + (PRIME2 * indexZ), seed);
}

ROSALIA__NOISE_DEF uint32_t get_4d_u32(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed)
{
    const int32_t PRIME1 = 198491317; // Large prime number with non-boring bits
    const int32_t PRIME2 = 6542989; // Large prime number with distinct and non-boring bits
    const int32_t PRIME3 = 357239; // Large prime number with distinct and non-boring bits
    return squirrelnoise5(indexX + (PRIME1 * indexY) + (PRIME2 * indexZ) + (PRIME3 * indexT), seed);
}

ROSALIA__NOISE_DEF float get_1d_zto(int32_t index, uint32_t seed)
{
    const double ONE_OVER_MAX_UINT = (1.0 / (double)0xFFFFFFFF);
    return (float)(ONE_OVER_MAX_UINT * (double)squirrelnoise5(index, seed));
}

ROSALIA__NOISE_DEF float get_2d_zto(int32_t indexX, int32_t indexY, uint32_t seed)
{
    const double ONE_OVER_MAX_UINT = (1.0 / (double)0xFFFFFFFF);
    return (float)(ONE_OVER_MAX_UINT * (double)get_2d_u32(indexX, indexY, seed));
}

ROSALIA__NOISE_DEF float get_3d_zto(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed)
{
    const double ONE_OVER_MAX_UINT = (1.0 / (double)0xFFFFFFFF);
    return (float)(ONE_OVER_MAX_UINT * (double)get_3d_u32(indexX, indexY, indexZ, seed));
}

ROSALIA__NOISE_DEF float get_4d_zto(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed)
{
    const double ONE_OVER_MAX_UINT = (1.0 / (double)0xFFFFFFFF);
    return (float)(ONE_OVER_MAX_UINT * (double)get_4d_u32(indexX, indexY, indexZ, indexT, seed));
}

ROSALIA__NOISE_DEF float get_1d_noto(int32_t index, uint32_t seed)
{
    const double ONE_OVER_MAX_INT = (1.0 / (double)0x7FFFFFFF);
    return (float)(ONE_OVER_MAX_INT * (double)(int)squirrelnoise5(index, seed));
}

ROSALIA__NOISE_DEF float get_2d_noto(int32_t indexX, int32_t indexY, uint32_t seed)
{
    const double ONE_OVER_MAX_INT = (1.0 / (double)0x7FFFFFFF);
    return (float)(ONE_OVER_MAX_INT * (double)(int)get_2d_u32(indexX, indexY, seed));
}

ROSALIA__NOISE_DEF float get_3d_noto(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed)
{
    const double ONE_OVER_MAX_INT = (1.0 / (double)0x7FFFFFFF);
    return (float)(ONE_OVER_MAX_INT * (double)(int)get_3d_u32(indexX, indexY, indexZ, seed));
}

ROSALIA__NOISE_DEF float get_4d_noto(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed)
{
    const double ONE_OVER_MAX_INT = (1.0 / (double)0x7FFFFFFF);
    return (float)(ONE_OVER_MAX_INT * (double)(int)get_4d_u32(indexX, indexY, indexZ, indexT, seed));
}

ROSALIA__NOISE_DEF uint32_t noise_get_uintn(int32_t position, uint32_t seed, uint32_t max_n)
{
    // https://funloop.org/post/2015-02-27-removing-modulo-bias-redux.html
    uint32_t r;
    uint32_t threshold = -max_n % max_n;
    do {
        r = squirrelnoise5(position, seed);
    } while (r < threshold);
    return r % max_n;
}

ROSALIA__NOISE_DEF uint32_t strhash(const char* str, const char* str_end)
{
    uint32_t acc = 0;
    if (!str_end) {
        str_end = str + strlen(str);
    }
    while (str < str_end) {
        acc *= squirrelnoise5(acc, *str);
        acc ^= squirrelnoise5(*str, acc);
        str++;
    }
    return acc;
}

// ROSALIA__NOISE_DEF float coherent_noise_2d(coherent_noise* cn, float x, float y)
// {
//     float max_amp = 0;
//     float amp = 1;
//     float freq = cn->scale;
//     float sample = 0;
//     // add successively smaller, higher-frequency terms
//     for (uint8_t i = 0; i < cn->octaves; i++) {
//         sample += get_2d_noto(x * freq, y * freq, cn->seed) * amp; //TODO use proper simplex noise, this doesnt work
//         max_amp += amp;
//         amp *= cn->persistence;
//         freq *= cn->lacunarity;
//     }
//     sample /= max_amp; // take the average value of the iterations
//     return sample;
// }

#ifdef __cplusplus
}
#endif

#endif
