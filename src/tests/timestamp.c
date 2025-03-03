#include <stdint.h>
#include <stdio.h>

#include "rosalia/test.h"
#include "rosalia/timestamp.h"
#include "rosalia/util.h"

#include "tests.h"

ROSA_TEST(timestamp)
{
    ROSA_TEST_CASE("ns64 and ms64 monotone increasing")
    {
        uint64_t monotone_ctr_ns = rosa_timestamp_get_ns64();
        uint64_t monotone_ctr_ms = rosa_timestamp_get_ms64();
        for (size_t i = 0; i < 32; i++) {
            uint64_t new_ctr_ns = rosa_timestamp_get_ns64();
            uint64_t new_ctr_ms = rosa_timestamp_get_ms64();
            rtassert(new_ctr_ns >= new_ctr_ns, NULL);
            rtassert(new_ctr_ms >= new_ctr_ms, NULL);
            monotone_ctr_ns = new_ctr_ns;
            monotone_ctr_ms = new_ctr_ms;
        }
    }
    ROSA_TEST_CASE("difference while running is >= 0")
    {
        rosa_timestamp run_ts = rosa_timestamp_get();
        for (size_t i = 0; i < 32; i++) {
            rosa_timestamp new_ts = rosa_timestamp_get();
            rtassert(rosa_timestamp_compare(new_ts, run_ts) >= 0, NULL);
            run_ts = new_ts;
        }
    }
    //TODO diff
    //TODO add
    //TODO timestamp fraction usage
}
