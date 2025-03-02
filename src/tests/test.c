

#include "rosalia/test.h"

#include "tests.h"

ROSA_TEST(test)
{
    ROSA_TEST_CASE("one")
    {
        (void)0;
    }
    ROSA_TEST_CASE("two")
    {
        int a = 1;
        int* b = NULL;
        a = *b;
    }
    ROSA_TEST_CASE("three")
    {
        (void)0;
    }
}
