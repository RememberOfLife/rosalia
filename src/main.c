#include <stdio.h>

#include "generated/git_commit_hash.h"

#include "tests/tests.h"

int main()
{
    printf("rosalia tests\n");
    printf("git commit hash: %s%s\n", GIT_COMMIT_HASH == NULL ? "<no commit info available>" : GIT_COMMIT_HASH, GIT_COMMIT_HASH != NULL && GIT_COMMIT_DIRTY ? " (dirty)" : "");

    run_test_argparse();
    run_test_base64();
    run_test_heapstackify();
    run_test_json();
    // run_test_log();
    run_test_math();
    run_test_noise();
    run_test_rand();
    run_test_semver();
    run_test_serialization();
    run_test_timestamp();
    run_test_vector();

    return 0;
}
