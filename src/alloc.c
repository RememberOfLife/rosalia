#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "rosalia/alloc.h"

#ifdef __cplusplus
extern "C" {
#endif

//TODO replace with the util version
static void* ptradd(void* p, size_t v)
{
    return (char*)p + v;
}

//TODO replace with the util version
static size_t ptrdiff(const void* p_end, const void* p_start)
{
    return (const char*)p_end - (const char*)p_start;
}

void* rosa_std_allocator_malloc(rosa_allocator* a, size_t s)
{
    return malloc(s);
}

void rosa_std_allocator_free(rosa_allocator* a, void* p)
{
    free(p);
}

rosa_allocator rosa_std_allocator = (rosa_allocator){
    .malloc = rosa_std_allocator_malloc,
    .free = rosa_std_allocator_free,
};

typedef struct rosa_allocator_linear_ctx_s {
    rosa_allocator* backing_alloc;
    void* base;
    void* stop;
    void* next;
} rosa_allocator_linear_ctx;

void* rosa_allocator_linear_malloc(rosa_allocator* a, size_t s)
{
    rosa_allocator_linear_ctx* ai = (rosa_allocator_linear_ctx*)a->context;
    if (ptrdiff(ai->stop, ai->next) < s) {
        return NULL;
    }
    void* rp = ai->next;
    ai->next = ptradd(ai->next, s);
    return rp;
}

void rosa_allocator_linear_free(rosa_allocator* a, void* p)
{
    // pass, this allocator doesn't free
}

rosa_allocator rosa_create_allocator_linear(rosa_allocator* backing_alloc, size_t total)
{
    rosa_allocator a = (rosa_allocator){
        .malloc = rosa_allocator_linear_malloc,
        .free = rosa_allocator_linear_free,
        .context = backing_alloc->malloc(backing_alloc, sizeof(rosa_allocator_linear_ctx)),
    };
    rosa_allocator_linear_ctx* ai = (rosa_allocator_linear_ctx*)a.context;
    ai->backing_alloc = backing_alloc;
    ai->base = backing_alloc->malloc(backing_alloc, total);
    ai->stop = ptradd(ai->base, total);
    ai->next = ai->base;
    return a;
}

void rosa_destroy_allocator_linear(rosa_allocator* a)
{
    rosa_allocator_linear_ctx* ai = (rosa_allocator_linear_ctx*)a->context;
    ai->backing_alloc->free(ai->backing_alloc, ai->base);
    a->malloc = NULL;
    a->free = NULL;
    ai->backing_alloc->free(ai->backing_alloc, ai);
    a->context = NULL;
}

#ifdef __cplusplus
}
#endif
