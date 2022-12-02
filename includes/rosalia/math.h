#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t rosa_log2(uint64_t v);

uint64_t rosa_ceil2pow2(uint64_t v);

uint64_t rosa_ceil2mult(uint64_t val, uint64_t mult);

uint64_t rosa_ceildiv(uint64_t div, uint64_t divisor);

uint64_t rosa_overlap(uint64_t value, uint64_t align);

#ifdef __cplusplus
}
#endif
