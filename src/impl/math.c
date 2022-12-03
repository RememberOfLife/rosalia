#include <stddef.h>
#include <stdint.h>

#include "rosalia/math.h"

uint64_t rosa_log2(uint64_t v)
{
    uint64_t res = 0;
    while (v > 0) {
        res++;
        v >>= 1;
    }
    return res;
}

uint64_t rosa_ceil2pow2(uint64_t v)
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

uint64_t rosa_ceil2mult(uint64_t val, uint64_t mult)
{
    uint64_t rem = val % mult;
    if (rem > 0) {
        return val + mult - rem;
    }
    return val;
}

uint64_t rosa_ceildiv(uint64_t div, uint64_t divisor)
{
    uint64_t rem = div / divisor;
    if (rem * divisor == div) {
        return rem;
    }
    return rem + 1;
}

uint64_t rosa_overlap(uint64_t value, uint64_t align)
{
    uint64_t rem = value % align;
    if (rem > 0) {
        return align - rem;
    }
    return 0;
}

float rosa_scale_f2f(float in_v, float in_min, float in_max, float out_min, float out_max)
{
    return (in_v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint64_t rosa_scale_f2i(float in_v, float in_min, float in_max, uint64_t out_min, uint64_t out_max)
{
    return (uint64_t)((in_v - in_min) * ((float)(out_max - out_min)) / (in_max - in_min) + (float)(out_min));
}

float rosa_scale_i2f(uint64_t in_v, uint64_t in_min, uint64_t in_max, float out_min, float out_max)
{
    return (float)(in_v - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

uint64_t rosa_scale_i2i(uint64_t in_v, uint64_t in_min, uint64_t in_max, uint64_t out_min, uint64_t out_max)
{
    return (in_v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
