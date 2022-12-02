#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <rosalia/alloc.h>
#include <rosalia/heapstackify.h>
#include <rosalia/timestamp.h>

#include "tests/tests.h"

uint64_t canon_fib(uint64_t v)
{
    if (v == 0) {
        return 0;
    }
    if (v == 1) {
        return 1;
    }
    return canon_fib(v - 1) + canon_fib(v - 2);
}

typedef struct fib_local_s {
    uint64_t l;
    uint64_t r;
} fib_local;

// HS_FUNC_DECL(fib, fib_local, uint64_t, uint64_t v);

HS_FUNC_DEF(fib, fib_local, uint64_t, uint64_t v)
{
    //TODO 2nd variant for local variables, use HS_FUNC_DEF_IL for that
    // HS_LOCALS(
    //     int l,
    //     int r,
    // );
    HS_BEGIN;
    if (p->v == 0) {
        HS_RETURN(0);
    }
    if (p->v == 1) {
        HS_RETURN(1);
    }
    HS_CALL_EMPLACE(&c->l, fib, p->v - 1);
    HS_CALL_EMPLACE(&c->r, fib, p->v - 2);
    HS_RETURN(c->l + c->r);
    HS_END;
}

uint64_t canon_runsum(uint64_t v)
{
    uint64_t locals[500];
    locals[v % 500] = v;
    if (v == 0) {
        return 0;
    }
    if (locals[v % 500] == 0) {
        locals[17] = 17;
    }
    return v + canon_runsum(v - 1);
}

typedef struct runsum_local_s {
    uint64_t locals[500];
    uint64_t r;
} runsum_local;

HS_FUNC_DEF(runsum, runsum_local, uint64_t, uint64_t v)
{
    HS_BEGIN;
    c->locals[p->v % 500] = p->v;
    if (p->v == 0) {
        HS_RETURN(0);
    }
    if (c->locals[p->v % 500] == 0) {
        c->locals[17] = 17;
    }
    HS_CALL_EMPLACE(&c->r, runsum, p->v - 1);
    HS_RETURN(p->v + c->r);
    HS_END;
}

void run_test_heapstackify()
{
    uint64_t start;
    uint64_t stop;
    uint64_t req_fib = 35;
    uint64_t result1;
    uint64_t result2;
    allocator lina;

    start = timestamp_get_ms64();
    result1 = canon_fib(req_fib);
    stop = timestamp_get_ms64();
    printf("canon fib %lu: %lu in %lums\n", req_fib, result1, stop - start);

    start = timestamp_get_ms64();
    HS_ENTRY_CALL_EMPLACE(&allocator_libc, &result2, fib, req_fib);
    stop = timestamp_get_ms64();
    printf("libc heapstackify fib %lu: %lu in %lums\n", req_fib, result2, stop - start);

    lina = create_allocator_linear((size_t)1 << 32);
    start = timestamp_get_ms64();
    HS_ENTRY_CALL_EMPLACE(&lina, &result2, fib, req_fib);
    stop = timestamp_get_ms64();
    printf("linear heapstackify fib %lu: %lu in %lums\n", req_fib, result2, stop - start);
    destroy_allocator_linear(&lina);

    printf("---\n");

    uint64_t req_rs = 100000;

    start = timestamp_get_ms64();
    HS_ENTRY_CALL_EMPLACE(&allocator_libc, &result1, runsum, req_rs);
    stop = timestamp_get_ms64();
    printf("libc heapstackify runsum %lu: %lu in %lums\n", req_rs, result1, stop - start);

    lina = create_allocator_linear((size_t)1 << 32);
    start = timestamp_get_ms64();
    HS_ENTRY_CALL_EMPLACE(&lina, &result1, runsum, req_rs);
    stop = timestamp_get_ms64();
    printf("linear heapstackify runsum %lu: %lu in %lums\n", req_rs, result1, stop - start);
    destroy_allocator_linear(&lina);

    // this overflows the stack with req_rs >= 10000
    printf("canon runsum %lu: ", req_rs);
    fflush(stdout);
    start = timestamp_get_ms64();
    result2 = canon_runsum(req_rs);
    stop = timestamp_get_ms64();
    printf("%luin %lums\n", result2, stop - start);
}
