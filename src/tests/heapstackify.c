#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "rosalia/alloc.h"
#include "rosalia/heapstackify.h"
#include "rosalia/timestamp.h"

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

int canon_ackerman(uint64_t m, uint64_t n)
{
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return canon_ackerman(m - 1, 1);
    else
        return canon_ackerman(m - 1, canon_ackerman(m, n - 1));
}

typedef struct runsum_local_s {
    uint64_t ret;
} runsum_local;

HS_FUNC_DEF(ackerman, runsum_local, uint64_t, uint64_t m, uint64_t n)
{
    HS_BEGIN;
    if (p->m == 0)
        HS_RETURN(p->n + 1);
    else if (p->n == 0) {
        HS_CALL_EMPLACE(&c->ret, ackerman, p->m - 1, 1);
        HS_RETURN(c->ret);
    } else {
        HS_CALL_EMPLACE(&c->ret, ackerman, p->m, p->n - 1);
        HS_CALL_EMPLACE(&c->ret, ackerman, p->m - 1, c->ret);
        HS_RETURN(c->ret);
    }
    HS_END;
}

void run_test_heapstackify()
{
    //TODO trim down for proper usage example

    uint64_t start;
    uint64_t stop;
    uint64_t req_fib = 10; //35;
    uint64_t result1;
    uint64_t result2;
    allocator lina;

    start = timestamp_get_ms64();
    result1 = canon_fib(req_fib);
    stop = timestamp_get_ms64();
    // printf("+ canon fib %lu: %lu in %lums\n", req_fib, result1, stop - start);

    start = timestamp_get_ms64();
    HS_ENTRY_CALL_EMPLACE(&allocator_libc, &result2, fib, req_fib);
    stop = timestamp_get_ms64();
    // printf("+ libc heapstackify fib %lu: %lu in %lums\n", req_fib, result2, stop - start);

    lina = create_allocator_linear((size_t)1 << 32);
    start = timestamp_get_ms64();
    HS_ENTRY_CALL_EMPLACE(&lina, &result2, fib, req_fib);
    stop = timestamp_get_ms64();
    // printf("+ linear heapstackify fib %lu: %lu in %lums\n", req_fib, result2, stop - start);
    destroy_allocator_linear(&lina);

    // printf("+ ---\n");

    uint64_t req_rs_m = 2; //3;
    uint64_t req_rs_n = 2; //11;

    start = timestamp_get_ms64();
    HS_ENTRY_CALL_EMPLACE(&allocator_libc, &result1, ackerman, req_rs_m, req_rs_n);
    stop = timestamp_get_ms64();
    // printf("+ libc heapstackify ackerman %lu %lu: %lu in %lums\n", req_rs_m, req_rs_n, result1, stop - start);

    lina = create_allocator_linear((size_t)1 << 34);
    start = timestamp_get_ms64();
    HS_ENTRY_CALL_EMPLACE(&lina, &result1, ackerman, req_rs_m, req_rs_n);
    stop = timestamp_get_ms64();
    // printf("+ linear heapstackify ackerman %lu %lu: %lu in %lums\n", req_rs_m, req_rs_n, result1, stop - start);
    destroy_allocator_linear(&lina);

    // printf("+ canon ackerman %lu %lu: ", req_rs_m, req_rs_n);
    fflush(stdout);
    start = timestamp_get_ms64();
    result2 = canon_ackerman(req_rs_m, req_rs_n);
    stop = timestamp_get_ms64();
    // printf("+ %lu in %lums\n", result2, stop - start);

    printf(". heapstackify\n");
}
