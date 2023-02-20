#ifndef ROSALIA_BASE64_H_INCLUDE
#define ROSALIA_BASE64_H_INCLUDE

// simple base64 alike implementation

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_BASE64_STATIC
#define ROSALIA__DEC static
#define ROSALIA__DEC_EXT static
#define ROSALIA__DEF static
#else
#define ROSALIA__DEC
#define ROSALIA__DEC_EXT extern
#define ROSALIA__DEF
#endif

#ifdef ROSALIA_BASE64_DECORATE
#define ROSALIA__DECORATE(ident) ROSALIA_BASE64_DECORATE(ident)
#else
#define ROSALIA__DECORATE(ident) ident
#endif

#define ROSALIA_BASE64_VERSION_MAJOR 0
#define ROSALIA_BASE64_VERSION_MINOR 1
#define ROSALIA_BASE64_VERSION_PATCH 4

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__DEC_EXT const char ROSALIA__DECORATE(b64_chars)[64];

ROSALIA__DEC_EXT const char ROSALIA__DECORATE(b64_inv)[80]; // = 'z' - '+' + 1

// returns the size (including null character) of the char buffer required to encode this many bytes
ROSALIA__DEC size_t ROSALIA__DECORATE(b64_encode_size)(size_t size_bytes);

// returns the size (byte count) produced by decoding this char buffer
ROSALIA__DEC size_t ROSALIA__DECORATE(b64_decode_size)(const char* data_chars);

// encode data_bytes as base64 into data_chars and returns the number of bytes written
ROSALIA__DEC size_t ROSALIA__DECORATE(b64_encode)(char* data_chars, const uint8_t* data_bytes, size_t len);

// decode data_chars base64 into data_bytes and returns the number of bytes decoded
ROSALIA__DEC size_t ROSALIA__DECORATE(b64_decode)(uint8_t* data_bytes, const char* data_chars);

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_BASE64_IMPLEMENTATION) && !defined(ROSALIA_BASE64_H_IMPL)
#define ROSALIA_BASE64_H_IMPL

#define ROSALIA__INTERNAL(ident) rosalia__internal_##ident

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO should probably switch to: https://github.com/superwills/NibbleAndAHalf

// code adapted for readability from: https://nachtimwald.com/2017/11/18/base64-encode-and-decode-in-c/

const char ROSALIA__DECORATE(b64_chars)[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-"; // not real base64 but much more usable

// shift input base64 char by '+' (43) then index into this table to get the 0-63 value represented by that char
const char ROSALIA__DECORATE(b64_inv)[80] = {62, -1, 63, -1, -1, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

ROSALIA__DEF size_t ROSALIA__DECORATE(b64_encode_size)(size_t size_bytes)
{
    // round up to nearest 3 multiple, div by 3 for 3 byte blocks and times 4 for the characters of each block
    size_t ret = size_bytes;
    if (size_bytes % 3 != 0) {
        ret += 3 - (size_bytes % 3);
    }
    return ((ret / 3) * 4);
}

ROSALIA__DEF size_t ROSALIA__DECORATE(b64_decode_size)(const char* data_chars)
{
    if (data_chars == NULL) {
        return 0;
    }
    size_t ret = 0;
    size_t pad = 0;
    const char* bp = data_chars;
    while (*bp != '\0') {
        if (*(bp++) == '=') {
            pad++;
        }
        ret++;
    }
    return ((ret / 4) * 3) - pad;
}

ROSALIA__DEF size_t ROSALIA__DECORATE(b64_encode)(char* data_chars, const uint8_t* data_bytes, size_t len)
{
    if (data_bytes == NULL || len == 0) {
        if (data_chars) {
            *data_chars = '\0';
        }
        return 0;
    }
    size_t bi = 0; // byte triple idx
    size_t ci = 0; // char quartet idx
    while (bi < len) {
        // accumulate 3 bytes of data
        uint32_t acc = (data_bytes[bi] << 16);
        if (bi + 1 < len) {
            acc |= (data_bytes[bi + 1] << 8);
        }
        if (bi + 2 < len) {
            acc |= data_bytes[bi + 2];
        }
        // produces at least 2 chars of data
        data_chars[ci] = ROSALIA__DECORATE(b64_chars)[(acc >> 18) & 0b111111];
        data_chars[ci + 1] = ROSALIA__DECORATE(b64_chars)[(acc >> 12) & 0b111111];
        // check if data exists or needs padding
        data_chars[ci + 2] = ((bi + 1 < len) ? ROSALIA__DECORATE(b64_chars)[(acc >> 6) & 0b111111] : '=');
        data_chars[ci + 3] = ((bi + 2 < len) ? ROSALIA__DECORATE(b64_chars)[acc & 0b111111] : '=');
        bi += 3;
        ci += 4;
    }
    data_chars[ci] = '\0';
    return ci + 1;
}

//TODO want some decorate internal for this?
ROSALIA__DEF bool ROSALIA__INTERNAL(b64_is_valid_char)(char c)
{
    if (c >= '0' && c <= '9') {
        return 1;
    }
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    if (c == '+' || c == '-' || c == '=') {
        return 1;
    }
    return 0;
}

ROSALIA__DEF size_t ROSALIA__DECORATE(b64_decode)(uint8_t* data_bytes, const char* data_chars)
{
    if (data_bytes == NULL || data_chars == NULL) {
        return 0;
    }
    //TODO move strlen and valid char into the decoding loop
    size_t strlen = 0;
    const char* cp = data_chars;
    while (*cp != '\0') {
        if (!ROSALIA__INTERNAL(b64_is_valid_char)(*(cp++))) {
            return 0;
        }
        strlen++;
    }
    if (strlen % 4 != 0) {
        return 0;
    }
    size_t ci = 0;
    size_t bi = 0;
    while (ci < strlen) {
        // writeout is interweaved with readin to accomodate fewer checks
        // magic number 43 comes from '+' which is the offset of the b64_inv array
        uint32_t v = (ROSALIA__DECORATE(b64_inv)[data_chars[ci] - 43] << 18);
        v |= (ROSALIA__DECORATE(b64_inv)[data_chars[ci + 1] - 43] << 12);
        data_bytes[bi] = ((v >> 16) & 0xFF); // writeout byte1
        bi++;
        if (data_chars[ci + 2] != '=') {
            v |= (ROSALIA__DECORATE(b64_inv)[data_chars[ci + 2] - 43] << 6);
            data_bytes[bi] = ((v >> 8) & 0xFF); // writeout byte2 (optional)
            bi++;
        }
        if (data_chars[ci + 3] != '=') {
            v |= ROSALIA__DECORATE(b64_inv)[data_chars[ci + 3] - 43];
            data_bytes[bi] = (v & 0xFF); // writeout byte3 (optional)
            bi++;
        }
        ci += 4;
    }
    return bi;
}

#ifdef __cplusplus
}
#endif

#undef ROSALIA__INTERNAL
#endif

#undef ROSALIA__DEC
#undef ROSALIA__DEC_EXT
#undef ROSALIA__DEF
#undef ROSALIA__DECORATE
