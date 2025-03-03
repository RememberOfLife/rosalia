#include <stdio.h>

#include "rosalia/test.h"

#include "tests.h"

#include "generated/git_commit_hash.h"

int main()
{
    printf("rosalia tests\n");
    printf("git commit hash: %s%s\n", GIT_COMMIT_HASH == NULL ? "<no commit info available>" : GIT_COMMIT_HASH, GIT_COMMIT_HASH != NULL && GIT_COMMIT_DIRTY ? " (dirty)" : "");

    //TODO NEEDS some way to specificy a single test and case, so we can run just that, and NOT fork, for easy gdb debuggability

    rosa_tester t;
    rosa_tester_create(&t, 64, 512);
    //TESTS-BEGIN
    ROSA_TESTER_ADD(&t, base64);
    ROSA_TESTER_ADD(&t, timestamp);
    ROSA_TESTER_ADD(&t, test);
    //TESTS-END
    size_t fail_count = rosa_tester_run(&t);
    printf("failed cases: %zu\n", fail_count);
    rosa_tester_destroy(&t);

    printf("DONE\n");
    return 0;
}
