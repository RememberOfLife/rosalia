#pragma once

// simple base64 with configurable chars 62/63

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

static const rosa_semver rosalia_noise_version = (rosa_semver){0, 2, 1};

// maps from 6 bit to chars and from any byte to 6-bit (or -1 if unused)
typedef struct rosa_b64_charset_s {
    char chars[64];
    char inverse[256];
} rosa_b64_charset;

// default charset uses [62]='+' and [63]'-'
extern const rosa_b64_charset rosa_b64_default_charset;
// specification charset uses [62]='+' and [63]'/'
extern const rosa_b64_charset rosa_b64_spec_charset;

// compute a dynamic charset from A-Z, a-z, 0-9 and the supplied char 62 and 63
void rosa_b64_compute_charset(rosa_b64_charset* charset, char char_62, char char_63);

// returns the size (including null character) of the char buffer required to encode this many bytes
size_t rosa_b64_encode_size(size_t size_bytes);

// returns the size (byte count) produced by decoding this char buffer
// if you know the length already, supply the str end to make this faster
size_t rosa_b64_decode_size(const char* data, const char* data_end);

// encode data_bytes as base64 into data_chars and returns the number of bytes written
size_t rosa_b64_encode(const rosa_b64_charset* cs, char* data_chars, const uint8_t* data_bytes, size_t len);

// decode data_chars base64 into data_bytes and returns the number of bytes decoded
size_t rosa_b64_decode(const rosa_b64_charset* cs, uint8_t* data_bytes, const char* data_chars);

#ifdef __cplusplus
}
#endif
