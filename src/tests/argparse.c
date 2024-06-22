#include <assert.h>
#include <stdio.h>
#include <string.h>

#define ROSALIA_ARGPARSE_IMPLEMENTATION
#include "rosalia/argparse.h"

#include "tests/tests.h"

void run_test_argparse()
{
    char* argv[] = {
        "program-name",
        "key1",
        "key2=val",
    };
    int argc = sizeof(argv) / sizeof(char*);

    rosa_argpv argpv;
    assert(rosa_argpv_create(&argpv, argc, argv));

    assert(rosa_argpv_exists(&argpv, "key1"));

    assert(rosa_argpv_val(&argpv, "key1") == NULL);
    assert(strcmp(rosa_argpv_val(&argpv, "key2"), "val") == 0);

    assert(strcmp(rosa_argpv_val_def(&argpv, "key1", "none"), "none") == 0);
    assert(strcmp(rosa_argpv_val_def(&argpv, "key2", "none"), "val") == 0);

    assert(rosa_argpv_val_eq(&argpv, "key1", NULL));
    assert(!rosa_argpv_val_eq(&argpv, "key1", "some"));
    assert(rosa_argpv_val_eq(&argpv, "key2", "val"));
    assert(!rosa_argpv_val_eq(&argpv, "key2", "other"));

    assert(rosa_argpv_find(&argpv, "key1") == 1);
    assert(rosa_argpv_find(&argpv, "key2") == 2);

    assert(strcmp(rosa_argpv_entry_at(&argpv, 0)->key, "program-name") == 0);
    assert(strcmp(rosa_argpv_entry_at(&argpv, 2)->val, "val") == 0);

    assert(rosa_argpv_entry_count(&argpv) == 3);

    rosa_argpv_destroy(&argpv);

    printf(". argparse\n");
}
