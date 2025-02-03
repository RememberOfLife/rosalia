#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t rosa_squirrelnoise5(int32_t position, uint32_t seed)
{
    const uint32_t SQ7_BIT_NOISE1 = 0xd2a80a3f; // 11010010101010000000101000111111
    const uint32_t SQ5_BIT_NOISE2 = 0xa884f197; // 10101000100001001111000110010111
    const uint32_t SQ5_BIT_NOISE3 = 0x6C736F4B; // 01101100011100110110111101001011
    const uint32_t SQ5_BIT_NOISE4 = 0xB79F3ABB; // 10110111100111110011101010111011
    const uint32_t SQ5_BIT_NOISE5 = 0x1b56c4f5; // 00011011010101101100010011110101
    uint32_t mangledBits = (uint32_t)position;
    mangledBits *= SQ7_BIT_NOISE1;
    mangledBits += seed;
    mangledBits ^= (mangledBits >> 9);
    mangledBits += SQ5_BIT_NOISE2;
    mangledBits ^= (mangledBits >> 11);
    mangledBits *= SQ5_BIT_NOISE3;
    mangledBits ^= (mangledBits >> 13);
    mangledBits += SQ5_BIT_NOISE4;
    mangledBits ^= (mangledBits >> 15);
    mangledBits *= SQ5_BIT_NOISE5;
    mangledBits ^= (mangledBits >> 17);
    return mangledBits;
}

uint32_t rosa_get_1d_u32(int32_t positionX, uint32_t seed)
{
    return rosa_squirrelnoise5(positionX, seed);
}

uint32_t rosa_get_2d_u32(int32_t index_x, int32_t index_y, uint32_t seed)
{
    const int32_t PRIME_NUMBER = 198491317; // Large prime number with non-boring bits
    return rosa_squirrelnoise5(index_x + (PRIME_NUMBER * index_y), seed);
}

uint32_t rosa_get_3d_u32(int32_t index_x, int32_t index_y, int32_t index_z, uint32_t seed)
{
    const int32_t PRIME1 = 198491317; // Large prime number with non-boring bits
    const int32_t PRIME2 = 6542989; // Large prime number with distinct and non-boring bits
    return rosa_squirrelnoise5(index_x + (PRIME1 * index_y) + (PRIME2 * index_z), seed);
}

uint32_t rosa_get_4d_u32(int32_t index_x, int32_t index_y, int32_t index_z, int32_t index_w, uint32_t seed)
{
    const int32_t PRIME1 = 198491317; // Large prime number with non-boring bits
    const int32_t PRIME2 = 6542989; // Large prime number with distinct and non-boring bits
    const int32_t PRIME3 = 357239; // Large prime number with distinct and non-boring bits
    return rosa_squirrelnoise5(index_x + (PRIME1 * index_y) + (PRIME2 * index_z) + (PRIME3 * index_w), seed);
}

float rosa_get_1d_zto(int32_t index, uint32_t seed)
{
    const double ONE_OVER_MAX_UINT = (1.0 / (double)0xFFFFFFFF);
    return (float)(ONE_OVER_MAX_UINT * (double)rosa_squirrelnoise5(index, seed));
}

float rosa_get_2d_zto(int32_t index_x, int32_t index_y, uint32_t seed)
{
    const double ONE_OVER_MAX_UINT = (1.0 / (double)0xFFFFFFFF);
    return (float)(ONE_OVER_MAX_UINT * (double)rosa_get_2d_u32(index_x, index_y, seed));
}

float rosa_get_3d_zto(int32_t index_x, int32_t index_y, int32_t index_z, uint32_t seed)
{
    const double ONE_OVER_MAX_UINT = (1.0 / (double)0xFFFFFFFF);
    return (float)(ONE_OVER_MAX_UINT * (double)rosa_get_3d_u32(index_x, index_y, index_z, seed));
}

float rosa_get_4d_zto(int32_t index_x, int32_t index_y, int32_t index_z, int32_t index_w, uint32_t seed)
{
    const double ONE_OVER_MAX_UINT = (1.0 / (double)0xFFFFFFFF);
    return (float)(ONE_OVER_MAX_UINT * (double)rosa_get_4d_u32(index_x, index_y, index_z, index_w, seed));
}

float rosa_get_1d_noto(int32_t index, uint32_t seed)
{
    const double ONE_OVER_MAX_INT = (1.0 / (double)0x7FFFFFFF);
    return (float)(ONE_OVER_MAX_INT * (double)(int)rosa_squirrelnoise5(index, seed));
}

float rosa_get_2d_noto(int32_t index_x, int32_t index_y, uint32_t seed)
{
    const double ONE_OVER_MAX_INT = (1.0 / (double)0x7FFFFFFF);
    return (float)(ONE_OVER_MAX_INT * (double)(int)rosa_get_2d_u32(index_x, index_y, seed));
}

float rosa_get_3d_noto(int32_t index_x, int32_t index_y, int32_t index_z, uint32_t seed)
{
    const double ONE_OVER_MAX_INT = (1.0 / (double)0x7FFFFFFF);
    return (float)(ONE_OVER_MAX_INT * (double)(int)rosa_get_3d_u32(index_x, index_y, index_z, seed));
}

float rosa_get_4d_noto(int32_t index_x, int32_t index_y, int32_t index_z, int32_t index_w, uint32_t seed)
{
    const double ONE_OVER_MAX_INT = (1.0 / (double)0x7FFFFFFF);
    return (float)(ONE_OVER_MAX_INT * (double)(int)rosa_get_4d_u32(index_x, index_y, index_z, index_w, seed));
}

uint32_t rosa_noise_get_uintn(int32_t position, uint32_t seed, uint32_t max_n)
{
    // https://funloop.org/post/2015-02-27-removing-modulo-bias-redux.html
    uint32_t r;
    uint32_t threshold = -max_n % max_n;
    do {
        r = rosa_squirrelnoise5(position, seed);
    } while (r < threshold);
    return r % max_n;
}

uint32_t rosa_strhash(const char* str, const char* str_end)
{
    uint32_t acc = 0;
    if (!str_end) {
        str_end = str + strlen(str);
    }
    while (str < str_end) {
        acc *= rosa_squirrelnoise5(acc, *str);
        acc ^= rosa_squirrelnoise5(*str, acc);
        str++;
    }
    return acc;
}

// float coherent_noise_2d(coherent_noise* cn, float x, float y)
// {
//     float max_amp = 0;
//     float amp = 1;
//     float freq = cn->scale;
//     float sample = 0;
//     // add successively smaller, higher-frequency terms
//     for (uint8_t i = 0; i < cn->octaves; i++) {
//         sample += get_2d_noto(x * freq, y * freq, cn->seed) * amp; //TODO use proper simplex noise, this doesnt work
//         max_amp += amp;
//         amp *= cn->persistence;
//         freq *= cn->lacunarity;
//     }
//     sample /= max_amp; // take the average value of the iterations
//     return sample;
// }

#ifdef __cplusplus
}
#endif
