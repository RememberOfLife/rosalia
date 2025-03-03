#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "rosalia/base64.h"
#include "rosalia/test.h"
#include "rosalia/util.h"

#include "tests.h"

ROSA_TEST(base64)
{
    ROSA_TEST_CASE("encode to decode is equal")
    {
        const int bytes_len = 256;
        uint8_t bytes1[bytes_len];
        uint8_t bytes2[bytes_len];
        char string1[bytes_len * 4];

        for (int i = 0; i < bytes_len; i++) {
            bytes1[i] = i;
        }
        size_t fill;

        fill = rosa_b64_encode_size(bytes_len);
        rtassert(fill > 0, NULL);

        fill = rosa_b64_encode(&rosa_b64_default_charset, string1, bytes1, bytes_len);
        rtassert(fill > 0, NULL);

        //BUG currently doesnt work
        fill = rosa_b64_decode_size(string1, NULL);
        rtassert(fill == bytes_len, NULL);

        fill = rosa_b64_decode(&rosa_b64_default_charset, bytes2, string1);
        rtassert(fill == bytes_len, NULL);

        rtassert(memcmp(bytes1, bytes2, bytes_len) == 0, NULL);
    }
}
