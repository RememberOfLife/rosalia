#include <stdint.h>
#include <stdio.h>

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
    rosa_argpv_create(&argpv, argc, argv);

    //TODO

    rosa_argpv_destroy(&argpv);

    printf(". argparse\n");
}
