#include <stdio.h>
#include <string.h>

#include "rosalia/argparse.h"
#include "rosalia/test.h"
#include "rosalia/util.h"

#include "tests.h"

ROSA_TEST(argparse)
{
    char* argv[] = {
        "program-name",
        "key1",
        "key2=val",
    };
    int argc = sizeof(argv) / sizeof(char*);

    rosa_argpv argpv;
    rtassert(rosa_argpv_create(&argpv, argc, argv), NULL);

    ROSA_TEST_CASE("key exists")
    {
        rtassert(rosa_argpv_exists(&argpv, "key1"), NULL);
    }
    ROSA_TEST_CASE("value by key")
    {
        rtassert(rosa_argpv_val(&argpv, "key1") == NULL, NULL);
        rtassert(strcmp(rosa_argpv_val(&argpv, "key2"), "val") == 0, NULL);
    }
    ROSA_TEST_CASE("value by key with default")
    {
        rtassert(strcmp(rosa_argpv_val_def(&argpv, "key1", "none"), "none") == 0, NULL);
        rtassert(strcmp(rosa_argpv_val_def(&argpv, "key2", "none"), "val") == 0, NULL);
    }
    ROSA_TEST_CASE("value is equal")
    {
        rtassert(rosa_argpv_val_eq(&argpv, "key1", NULL), NULL);
        rtassert(!rosa_argpv_val_eq(&argpv, "key1", "some"), NULL);
        rtassert(rosa_argpv_val_eq(&argpv, "key2", "val"), NULL);
        rtassert(!rosa_argpv_val_eq(&argpv, "key2", "other"), NULL);
    }
    ROSA_TEST_CASE("find key index")
    {
        rtassert(rosa_argpv_find(&argpv, "key1") == 1, NULL);
        rtassert(rosa_argpv_find(&argpv, "key2") == 2, NULL);
    }
    ROSA_TEST_CASE("get entry at index")
    {
        rtassert(strcmp(rosa_argpv_entry_at(&argpv, 0)->key, "program-name") == 0, NULL);
        rtassert(strcmp(rosa_argpv_entry_at(&argpv, 2)->val, "val") == 0, NULL);
    }
    ROSA_TEST_CASE("entry count")
    {
        rtassert(rosa_argpv_entry_count(&argpv) == 3, NULL);
    }

    rosa_argpv_destroy(&argpv);
}
