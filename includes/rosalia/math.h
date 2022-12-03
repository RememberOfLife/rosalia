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

//TODO double versions
//TODO check that they work properly, especially for the non f2f versions
float rosa_scale_f2f(float in_v, float in_min, float in_max, float out_min, float out_max);
uint64_t rosa_scale_f2i(float in_v, float in_min, float in_max, uint64_t out_min, uint64_t out_max);
float rosa_scale_i2f(uint64_t in_v, uint64_t in_min, uint64_t in_max, float out_min, float out_max);
uint64_t rosa_scale_i2i(uint64_t in_v, uint64_t in_min, uint64_t in_max, uint64_t out_min, uint64_t out_max);

//TODO Clamp TresholdEq

#ifdef __cplusplus
}
#endif
