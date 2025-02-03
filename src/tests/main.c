#include <stdio.h>

#include "generated/git_commit_hash.h"

int main()
{
    printf("rosalia tests\n");
    printf("git commit hash: %s%s\n", GIT_COMMIT_HASH == NULL ? "<no commit info available>" : GIT_COMMIT_HASH, GIT_COMMIT_HASH != NULL && GIT_COMMIT_DIRTY ? " (dirty)" : "");

    printf("DONE\n");
    return 0;
}
