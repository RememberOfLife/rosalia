#include "tests/tests.h"

int main()
{
    run_test_base64();
    run_test_heapstackify();
    run_test_noise();
    run_test_rand();
    run_test_raw_stream();
    run_test_semver();
    run_test_serialization();
    return 0;
}
