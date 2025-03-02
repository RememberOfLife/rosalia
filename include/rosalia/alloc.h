#pragma once

#include <stddef.h>
#include <stdint.h>

#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

static const rosa_semver rosalia_alloc_version = (rosa_semver){0, 1, 0};

//TODO shorten name to alloc?

// consider block based allocations?
/*
typedef struct {
    void* data;
    size_t len;
} mblk;
*/

//TODO aligned alloc feature? etc.. and util for zeroing etc..

typedef struct rosa_allocator_s rosa_allocator;

struct rosa_allocator_s {
    void* (*malloc)(rosa_allocator* a, size_t s);
    // void* (*realloc)(rosa_allocator* a, void* p, size_t s);
    void (*free)(rosa_allocator* a, void* p);
    void* context;
};

extern rosa_allocator rosa_std_allocator;

rosa_allocator rosa_create_allocator_linear(rosa_allocator* backing_alloc, size_t total);
void rosa_destroy_allocator_linear(rosa_allocator* a);

// slaballocators and pools and arena allocator

#ifdef __cplusplus
extern "C"
}
#endif
