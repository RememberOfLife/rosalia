#ifndef ROSALIA_MATH_H_INCLUDE
#define ROSALIA_MATH_H_INCLUDE

#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_MATH_STATIC
#define ROSALIA__MATH_DEC static
#define ROSALIA__MATH_DEC_EXT static
#define ROSALIA__MATH_DEF static
#else
#define ROSALIA__MATH_DEC
#define ROSALIA__MATH_DEC_EXT extern
#define ROSALIA__MATH_DEF
#endif

#ifdef ROSALIA_MATH_DECORATE
#define ROSALIA__MATH_DECORATE(ident) ROSALIA_MATH_DECORATE(ident)
#else
#define ROSALIA__MATH_DECORATE(ident) rosa_##ident
#endif

#define ROSALIA_MATH_VERSION_MAJOR 0
#define ROSALIA_MATH_VERSION_MINOR 3
#define ROSALIA_MATH_VERSION_PATCH 2

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__MATH_DEC uint64_t ROSALIA__MATH_DECORATE(log2)(uint64_t v);

ROSALIA__MATH_DEC uint64_t ROSALIA__MATH_DECORATE(ceil2pow2)(uint64_t v);

ROSALIA__MATH_DEC uint64_t ROSALIA__MATH_DECORATE(ceil2mult)(uint64_t val, uint64_t mult);

ROSALIA__MATH_DEC uint64_t ROSALIA__MATH_DECORATE(ceildiv)(uint64_t div, uint64_t divisor);

ROSALIA__MATH_DEC uint64_t ROSALIA__MATH_DECORATE(overlap)(uint64_t value, uint64_t align);

//TODO double versions
//TODO check that they work properly, especially for the non f2f versions
ROSALIA__MATH_DEC float ROSALIA__MATH_DECORATE(scale_f2f)(float in_v, float in_min, float in_max, float out_min, float out_max);
ROSALIA__MATH_DEC uint64_t ROSALIA__MATH_DECORATE(scale_f2i)(float in_v, float in_min, float in_max, uint64_t out_min, uint64_t out_max);
ROSALIA__MATH_DEC float ROSALIA__MATH_DECORATE(scale_i2f)(uint64_t in_v, uint64_t in_min, uint64_t in_max, float out_min, float out_max);
ROSALIA__MATH_DEC uint64_t ROSALIA__MATH_DECORATE(scale_i2i)(uint64_t in_v, uint64_t in_min, uint64_t in_max, uint64_t out_min, uint64_t out_max);

//TODO Clamp TresholdEq

//TODO float + epsilon (smallest value increment)

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_MATH_IMPLEMENTATION) && !defined(ROSALIA_MATH_H_IMPL)
#define ROSALIA_MATH_H_IMPL

#define ROSALIA__MATH_INTERNAL(ident) rosalia__math_internal_##ident

#include <stddef.h>
#include <stdint.h>

ROSALIA__MATH_DEF uint64_t ROSALIA__MATH_DECORATE(log2)(uint64_t v)
{
    uint64_t res = 0;
    while (v > 0) {
        res++;
        v >>= 1;
    }
    return res;
}

ROSALIA__MATH_DEF uint64_t ROSALIA__MATH_DECORATE(ceil2pow2)(uint64_t v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;
    return v;
}

ROSALIA__MATH_DEF uint64_t ROSALIA__MATH_DECORATE(ceil2mult)(uint64_t val, uint64_t mult)
{
    uint64_t rem = val % mult;
    if (rem > 0) {
        return val + mult - rem;
    }
    return val;
}

ROSALIA__MATH_DEF uint64_t ROSALIA__MATH_DECORATE(ceildiv)(uint64_t div, uint64_t divisor)
{
    uint64_t rem = div / divisor;
    if (rem * divisor == div) {
        return rem;
    }
    return rem + 1;
}

ROSALIA__MATH_DEF uint64_t ROSALIA__MATH_DECORATE(overlap)(uint64_t value, uint64_t align)
{
    uint64_t rem = value % align;
    if (rem > 0) {
        return align - rem;
    }
    return 0;
}

ROSALIA__MATH_DEF float ROSALIA__MATH_DECORATE(scale_f2f)(float in_v, float in_min, float in_max, float out_min, float out_max)
{
    return (in_v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

ROSALIA__MATH_DEF uint64_t ROSALIA__MATH_DECORATE(scale_f2i)(float in_v, float in_min, float in_max, uint64_t out_min, uint64_t out_max)
{
    return (uint64_t)((in_v - in_min) * ((float)(out_max - out_min)) / (in_max - in_min) + (float)(out_min));
}

ROSALIA__MATH_DEF float ROSALIA__MATH_DECORATE(scale_i2f)(uint64_t in_v, uint64_t in_min, uint64_t in_max, float out_min, float out_max)
{
    return (float)(in_v - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

ROSALIA__MATH_DEF uint64_t ROSALIA__MATH_DECORATE(scale_i2i)(uint64_t in_v, uint64_t in_min, uint64_t in_max, uint64_t out_min, uint64_t out_max)
{
    return (in_v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif
