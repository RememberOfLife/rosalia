#include <stdint.h>
#include <stdio.h>

#define ROSALIA_TIMESTAMP_IMPLEMENTATION
#include "rosalia/timestamp.h"

#include "tests/tests.h"

void run_test_timestamp()
{
    uint64_t monotone_ctr_ns = timestamp_get_ns64();
    uint64_t monotone_ctr_ms = timestamp_get_ms64();
    for (size_t i = 0; i < 32; i++) {
        uint64_t new_ctr_ns = timestamp_get_ns64();
        uint64_t new_ctr_ms = timestamp_get_ms64();
        assert(new_ctr_ns >= new_ctr_ns);
        assert(new_ctr_ms >= new_ctr_ms);
        monotone_ctr_ns = new_ctr_ns;
        monotone_ctr_ms = new_ctr_ms;
    }

    timestamp run_ts = timestamp_get();
    for (size_t i = 0; i < 32; i++) {
        timestamp new_ts = timestamp_get();
        assert(timestamp_compare(new_ts, run_ts) >= 0);
        run_ts = new_ts;
    }

    //TODO timestamp fraction usage

    printf(". timestamp\n");
}
