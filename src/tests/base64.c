#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ROSALIA_BASE64_IMPLEMENTATION
#include "rosalia/base64.h"

#include "tests/tests.h"

void run_test_base64()
{
    const int bytes_len = 256;
    uint8_t bytes1[256];
    uint8_t bytes2[256];
    char string1[1024];

    for (int i = 0; i < bytes_len; i++) {
        bytes1[i] = i;
    }
    size_t fill;

    fill = b64_encode_size(bytes_len);
    assert(fill > 0);

    fill = b64_encode(string1, bytes1, bytes_len);
    assert(fill > 0);

    fill = b64_decode_size(string1);
    assert(fill == bytes_len);

    fill = b64_decode(bytes2, string1);
    assert(fill == bytes_len);

    assert(memcmp(bytes1, bytes2, bytes_len) == 0);

    printf(". base64\n");
}
