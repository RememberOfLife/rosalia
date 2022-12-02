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
