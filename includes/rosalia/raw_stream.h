#ifndef ROSALIA_RAW_STREAM_H_INCLUDE
#define ROSALIA_RAW_STREAM_H_INCLUDE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_RAW_STREAM_STATIC
#define ROSALIA__DEC static
#define ROSALIA__DEC_EXT static
#define ROSALIA__DEF static
#else
#define ROSALIA__DEC
#define ROSALIA__DEC_EXT extern
#define ROSALIA__DEF
#endif

#ifdef ROSALIA_RAW_STREAM_DECORATE
#define ROSALIA__DECORATE(ident) ROSALIA_RAW_STREAM_DECORATE(ident)
#else
#define ROSALIA__DECORATE(ident) ident
#endif

#define ROSALIA_RAW_STREAM_VERSION_MAJOR 0
#define ROSALIA_RAW_STREAM_VERSION_MINOR 1
#define RSOALIA_RAW_STREAM_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

//TODO ROSALIA__DECORATE this struct
typedef struct raw_stream_s {
    uint8_t* begin;
    uint8_t* end;
} raw_stream;

ROSALIA__DEC raw_stream ROSALIA__DECORATE(rs_init)(uint8_t* buf);
ROSALIA__DEC size_t ROSALIA__DECORATE(rs_get_size)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_bool)(raw_stream* rs, bool v);
ROSALIA__DEC bool ROSALIA__DECORATE(rs_r_bool)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_int8)(raw_stream* rs, int8_t v);
ROSALIA__DEC int8_t ROSALIA__DECORATE(rs_r_int8)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_int16)(raw_stream* rs, int16_t v);
ROSALIA__DEC int16_t ROSALIA__DECORATE(rs_r_int16)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_int32)(raw_stream* rs, int32_t v);
ROSALIA__DEC int32_t ROSALIA__DECORATE(rs_r_int32)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_int64)(raw_stream* rs, int64_t v);
ROSALIA__DEC int64_t ROSALIA__DECORATE(rs_r_int64)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_uint8)(raw_stream* rs, uint8_t v);
ROSALIA__DEC uint8_t ROSALIA__DECORATE(rs_r_uint8)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_uint16)(raw_stream* rs, uint16_t v);
ROSALIA__DEC uint16_t ROSALIA__DECORATE(rs_r_uint16)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_uint32)(raw_stream* rs, uint32_t v);
ROSALIA__DEC uint32_t ROSALIA__DECORATE(rs_r_uint32)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_uint64)(raw_stream* rs, uint64_t v);
ROSALIA__DEC uint64_t ROSALIA__DECORATE(rs_r_uint64)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_size)(raw_stream* rs, size_t v);
ROSALIA__DEC size_t ROSALIA__DECORATE(rs_r_size)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_float)(raw_stream* rs, float v);
ROSALIA__DEC float ROSALIA__DECORATE(rs_r_float)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_double)(raw_stream* rs, double v);
ROSALIA__DEC double ROSALIA__DECORATE(rs_r_double)(raw_stream* rs);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_string)(raw_stream* rs, const char* str, const char* str_end);
ROSALIA__DEC size_t ROSALIA__DECORATE(rs_r_string_size)(raw_stream* rs); // does NOT alter the stream position; DOES account for the null character
ROSALIA__DEC void ROSALIA__DECORATE(rs_r_string)(raw_stream* rs, char* str);

ROSALIA__DEC void ROSALIA__DECORATE(rs_w_raw)(raw_stream* rs, const uint8_t* buf, size_t size);
ROSALIA__DEC size_t ROSALIA__DECORATE(rs_r_raw_size)(raw_stream* rs); // does NOT alter the stream position
ROSALIA__DEC void ROSALIA__DECORATE(rs_r_raw)(raw_stream* rs, uint8_t* buf);

#ifdef __cplusplus
}
#endif

#endif

#ifdef ROSALIA_RAW_STREAM_IMPLEMENTATION

#define ROSALIA__INTERNAL(ident) rosalia__internal_##ident

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO simplify RAW_STREAM encoding process for byte based primitives (int/uint/float etc) by just using one, "encode bytes" func, on a union type!

ROSALIA__DEF raw_stream ROSALIA__DECORATE(rs_init)(uint8_t* buf)
{
    return (raw_stream){
        .begin = buf,
        .end = buf,
    };
}

ROSALIA__DEF size_t ROSALIA__DECORATE(rs_get_size)(raw_stream* rs)
{
    return rs->end - rs->begin;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_bool)(raw_stream* rs, bool v)
{
    rs->end[0] = (uint8_t)v;
    rs->end++;
}

ROSALIA__DEF bool ROSALIA__DECORATE(rs_r_bool)(raw_stream* rs)
{
    bool r = (bool)(rs->end[0]);
    rs->end++;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_int8)(raw_stream* rs, int8_t v)
{
    rs->end[0] = (uint8_t)v;
    rs->end++;
}

ROSALIA__DEF int8_t ROSALIA__DECORATE(rs_r_int8)(raw_stream* rs)
{
    int8_t r = (int8_t)(rs->end[0]);
    rs->end++;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_int16)(raw_stream* rs, int16_t v)
{
    rs->end[0] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 2;
}

ROSALIA__DEF int16_t ROSALIA__DECORATE(rs_r_int16)(raw_stream* rs)
{
    int16_t r = 0;
    r |= (int16_t)(rs->end[0]) << 8;
    r |= (int16_t)(rs->end[1]) << 0;
    rs->end += 2;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_int32)(raw_stream* rs, int32_t v)
{
    rs->end[0] = (uint8_t)((v >> 24) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 16) & 0xFF);
    rs->end[2] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[3] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 4;
}

ROSALIA__DEF int32_t ROSALIA__DECORATE(rs_r_int32)(raw_stream* rs)
{
    int32_t r = 0;
    r |= (int32_t)(rs->end[0]) << 24;
    r |= (int32_t)(rs->end[1]) << 16;
    r |= (int32_t)(rs->end[2]) << 8;
    r |= (int32_t)(rs->end[3]) << 0;
    rs->end += 4;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_int64)(raw_stream* rs, int64_t v)
{
    rs->end[0] = (uint8_t)((v >> 56) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 48) & 0xFF);
    rs->end[2] = (uint8_t)((v >> 40) & 0xFF);
    rs->end[3] = (uint8_t)((v >> 32) & 0xFF);
    rs->end[4] = (uint8_t)((v >> 24) & 0xFF);
    rs->end[5] = (uint8_t)((v >> 16) & 0xFF);
    rs->end[6] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[7] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 8;
}

ROSALIA__DEF int64_t ROSALIA__DECORATE(rs_r_int64)(raw_stream* rs)
{
    int64_t r = 0;
    r |= (int64_t)(rs->end[0]) << 56;
    r |= (int64_t)(rs->end[1]) << 48;
    r |= (int64_t)(rs->end[2]) << 40;
    r |= (int64_t)(rs->end[3]) << 32;
    r |= (int64_t)(rs->end[4]) << 24;
    r |= (int64_t)(rs->end[5]) << 16;
    r |= (int64_t)(rs->end[6]) << 8;
    r |= (int64_t)(rs->end[7]) << 0;
    rs->end += 8;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_uint8)(raw_stream* rs, uint8_t v)
{
    rs->end[0] = (uint8_t)v;
    rs->end++;
}

ROSALIA__DEF uint8_t ROSALIA__DECORATE(rs_r_uint8)(raw_stream* rs)
{
    uint8_t r = (uint8_t)(rs->end[0]);
    rs->end++;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_uint16)(raw_stream* rs, uint16_t v)
{
    rs->end[0] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 2;
}

ROSALIA__DEF uint16_t ROSALIA__DECORATE(rs_r_uint16)(raw_stream* rs)
{
    uint16_t r = 0;
    r |= (uint16_t)(rs->end[0]) << 8;
    r |= (uint16_t)(rs->end[1]) << 0;
    rs->end += 2;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_uint32)(raw_stream* rs, uint32_t v)
{
    rs->end[0] = (uint8_t)((v >> 24) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 16) & 0xFF);
    rs->end[2] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[3] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 4;
}

ROSALIA__DEF uint32_t ROSALIA__DECORATE(rs_r_uint32)(raw_stream* rs)
{
    uint32_t r = 0;
    r |= (uint32_t)(rs->end[0]) << 24;
    r |= (uint32_t)(rs->end[1]) << 16;
    r |= (uint32_t)(rs->end[2]) << 8;
    r |= (uint32_t)(rs->end[3]) << 0;
    rs->end += 4;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_uint64)(raw_stream* rs, uint64_t v)
{
    rs->end[0] = (uint8_t)((v >> 56) & 0xFF);
    rs->end[1] = (uint8_t)((v >> 48) & 0xFF);
    rs->end[2] = (uint8_t)((v >> 40) & 0xFF);
    rs->end[3] = (uint8_t)((v >> 32) & 0xFF);
    rs->end[4] = (uint8_t)((v >> 24) & 0xFF);
    rs->end[5] = (uint8_t)((v >> 16) & 0xFF);
    rs->end[6] = (uint8_t)((v >> 8) & 0xFF);
    rs->end[7] = (uint8_t)((v >> 0) & 0xFF);
    rs->end += 8;
}

ROSALIA__DEF uint64_t ROSALIA__DECORATE(rs_r_uint64)(raw_stream* rs)
{
    uint64_t r = 0;
    r |= (uint64_t)(rs->end[0]) << 56;
    r |= (uint64_t)(rs->end[1]) << 48;
    r |= (uint64_t)(rs->end[2]) << 40;
    r |= (uint64_t)(rs->end[3]) << 32;
    r |= (uint64_t)(rs->end[4]) << 24;
    r |= (uint64_t)(rs->end[5]) << 16;
    r |= (uint64_t)(rs->end[6]) << 8;
    r |= (uint64_t)(rs->end[7]) << 0;
    rs->end += 8;
    return r;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_size)(raw_stream* rs, size_t v)
{
    ROSALIA__DECORATE(rs_w_uint64)
    (rs, (uint64_t)(v));
}

ROSALIA__DEF size_t ROSALIA__DECORATE(rs_r_size)(raw_stream* rs)
{
    return (size_t)ROSALIA__DECORATE(rs_r_uint64)(rs);
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_float)(raw_stream* rs, float v)
{
    union {
        float f;
        uint8_t u8[sizeof(float)];
    } f32 = {.f = v};

    for (int i = 0; i < sizeof(float); i++) {
        ROSALIA__DECORATE(rs_w_uint8)
        (rs, f32.u8[i]);
    }
}

ROSALIA__DEF float ROSALIA__DECORATE(rs_r_float)(raw_stream* rs)
{
    union {
        float f;
        uint8_t u8[sizeof(float)];
    } f32;

    for (int i = 0; i < sizeof(float); i++) {
        f32.u8[i] = ROSALIA__DECORATE(rs_r_uint8)(rs);
    }
    return f32.f;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_double)(raw_stream* rs, double v)
{
    union {
        double d;
        uint8_t u8[sizeof(double)];
    } f64 = {.d = v};

    for (int i = 0; i < sizeof(double); i++) {
        ROSALIA__DECORATE(rs_w_uint8)
        (rs, f64.u8[i]);
    }
}

ROSALIA__DEF double ROSALIA__DECORATE(rs_r_double)(raw_stream* rs)
{
    union {
        double d;
        uint8_t u8[sizeof(double)];
    } f64;

    for (int i = 0; i < sizeof(double); i++) {
        f64.u8[i] = ROSALIA__DECORATE(rs_r_uint8)(rs);
    }
    return f64.d;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_string)(raw_stream* rs, const char* str, const char* str_end)
{
    size_t size;
    if (str_end == NULL) {
        size = strlen(str) + 1;
    } else {
        size = str_end - str;
    }
    memcpy(rs->end, str, size);
    rs->end += size;
}

ROSALIA__DEF size_t ROSALIA__DECORATE(rs_r_string_size)(raw_stream* rs)
{
    return strlen((char*)rs->end) + 1;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_r_string)(raw_stream* rs, char* str)
{
    size_t len = strlen((char*)rs->end) + 1;
    memcpy(str, rs->end, len);
    rs->end += len;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_w_raw)(raw_stream* rs, const uint8_t* buf, size_t size)
{
    ROSALIA__DECORATE(rs_w_size)
    (rs, size);
    memcpy(rs->end, buf, size);
    rs->end += size;
}

ROSALIA__DEF size_t ROSALIA__DECORATE(rs_r_raw_size)(raw_stream* rs)
{
    size_t size = ROSALIA__DECORATE(rs_r_size)(rs);
    rs->end -= sizeof(uint64_t); // this function should not alter the stream position
    return size;
}

ROSALIA__DEF void ROSALIA__DECORATE(rs_r_raw)(raw_stream* rs, uint8_t* buf)
{
    size_t size = ROSALIA__DECORATE(rs_r_size)(rs);
    memcpy(buf, rs->end, size);
    rs->end += size;
}

#ifdef __cplusplus
}
#endif

#undef ROSALIA_RAW_STREAM_IMPLEMENTATION
#undef ROSALIA__INTERNAL
#endif

#undef ROSALIA__DEC
#undef ROSALIA__DEC_EXT
#undef ROSALIA__DEF
#undef ROSALIA__DECORATE
