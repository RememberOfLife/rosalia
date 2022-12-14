#pragma once

#include <stddef.h>
#include <stdint.h>

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
uint32_t squirrelnoise5(int32_t positionX, uint32_t seed);

// raw pseudorandom noise functions (random-access / deterministic), basis of all other noise
uint32_t get_1d_u32(int32_t index, uint32_t seed);
uint32_t get_2d_u32(int32_t indexX, int32_t indexY, uint32_t seed);
uint32_t get_3d_u32(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed);
uint32_t get_4d_u32(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed);

// same nois, mapped to floats in [0,1] for convenience
float get_1d_zto(int32_t index, uint32_t seed);
float get_2d_zto(int32_t indexX, int32_t indexY, uint32_t seed);
float get_3d_zto(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed);
float get_4d_zto(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed);

// same nois, mapped to floats in [-1,1] for convenience
float get_1d_noto(int32_t index, uint32_t seed);
float get_2d_noto(int32_t indexX, int32_t indexY, uint32_t seed);
float get_3d_noto(int32_t indexX, int32_t indexY, int32_t indexZ, uint32_t seed);
float get_4d_noto(int32_t indexX, int32_t indexY, int32_t indexZ, int32_t indexT, uint32_t seed);

// strhash function not part of squirrelnoise
// pass str_end NULL as default to use normal null character termination
// str_end points to the first character NOT included in the hash
uint32_t strhash(const char* str, const char* str_end);

typedef struct coherent_noise_s {
    uint32_t seed;
    float scale; // what distance to view the noisemap from, smaller is closer (i.e. structures get bigger)
    uint8_t octaves; // levels of detail noise should have, higher gives more possible detail (finer grain)
    float lacunarity; // how much detail is added or removed each octave (adjusts frequency), higher gives less blending of octaves
    float persistence; // how much each octave contributes to the overall shape (adjusts amplitude), higher makes rougher
} coherent_noise;

// returns in [-1,1]
float coherent_noise_2d(coherent_noise* cn, float x, float y);

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
