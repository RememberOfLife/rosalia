#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct allocator_s allocator;

struct allocator_s {
    void* (*malloc)(allocator* a, size_t s);
    void (*free)(allocator* a, void* p);
    uint8_t reserved[48];
};

extern allocator allocator_libc;

allocator create_allocator_linear(size_t total);
void destroy_allocator_linear(allocator* a);

// slaballocators and pools and arena allocator

#ifdef __cplusplus
extern "C"
}
#endif
