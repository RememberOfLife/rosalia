#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "rosalia/alloc.h"
#include "rosalia/serialization.h"

#ifdef __cplusplus
extern "C" {
#endif

void* libc_alloc_malloc(allocator* a, size_t s)
{
    return malloc(s);
}

void libc_alloc_free(allocator* a, void* p)
{
    free(p);
}

allocator allocator_libc = (allocator){
    .malloc = libc_alloc_malloc,
    .free = libc_alloc_free,
};

typedef struct linear_alloc_s {
    void* base;
    void* stop;
    void* next;
} linear_alloc;

void* linear_alloc_malloc(allocator* a, size_t s)
{
    linear_alloc* ai = (linear_alloc*)a->context;
    if (ptrdiff(ai->stop, ai->next) < s) {
        return NULL;
    }
    void* rp = ai->next;
    ai->next = ptradd(ai->next, s);
    return rp;
}

void linear_alloc_free(allocator* a, void* p)
{
    // pass, this allocator doesn't free
}

allocator create_allocator_linear(size_t total)
{
    allocator a = (allocator){
        .malloc = linear_alloc_malloc,
        .free = linear_alloc_free,
        .context = malloc(sizeof(linear_alloc)),
    };
    linear_alloc* ai = (linear_alloc*)a.context;
    ai->base = malloc(total);
    ai->stop = ptradd(ai->base, total);
    ai->next = ai->base;
    return a;
}

void destroy_allocator_linear(allocator* a)
{
    linear_alloc* ai = (linear_alloc*)a->context;
    free(ai->base);
    a->malloc = NULL;
    a->free = NULL;
    free(ai);
}

#ifdef __cplusplus
}
#endif
