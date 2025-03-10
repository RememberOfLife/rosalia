#include <stdint.h>
#include <stdio.h>

#include "rosalia/arraylist.h"
#include "rosalia/test.h"
#include "rosalia/util.h"

#include "tests.h"

ROSA_TEST(arraylist)
{
    ROSA_TEST_CASE("general")
    {
        ROSA_ARRAYLIST(uint32_t) v1;
        ROSA_ARRAYLIST_CREATE(&v1, 0);
        for (int i = 0; i < 10; i++) {
            ROSA_ARRAYLIST_PUSH(&v1, i);
        }
        rtassert(ROSA_ARRAYLIST_LEN(&v1) == 10, NULL);
        rtassert(ROSA_ARRAYLIST_CAP(&v1) >= 10, NULL);
        for (int i = 0; i < ROSA_ARRAYLIST_LEN(&v1); i++) {
            rtassert(v1[i] == i, NULL);
        }
        ROSA_ARRAYLIST_DESTROY(&v1);
        rtassert(v1 == NULL, NULL);
    }
}
