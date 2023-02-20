#ifndef ROSALIA_SEMVER_H_INCLUDE
#define ROSALIA_SEMVER_H_INCLUDE

// provides very limited subset of semver

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_SEMVER_STATIC
#define ROSALIA__SEMVER_DEC static
#define ROSALIA__SEMVER_DEC_EXT static
#define ROSALIA__SEMVER_DEF static
#else
#define ROSALIA__SEMVER_DEC
#define ROSALIA__SEMVER_DEC_EXT extern
#define ROSALIA__SEMVER_DEF
#endif

#ifdef ROSALIA_SEMVER_DECORATE
#define ROSALIA__SEMVER_DECORATE(ident) ROSALIA_SEMVER_DECORATE(ident)
#else
#define ROSALIA__SEMVER_DECORATE(ident) ident
#endif

#define ROSALIA_SEMVER_VERSION_MAJOR 0
#define ROSALIA_SEMVER_VERSION_MINOR 1
#define ROSALIA_SEMVER_VERSION_PATCH 5

#ifdef __cplusplus
extern "C" {
#endif

//TODO ROSALIA__SEMVER_DECORATE for this struct and _ext
typedef struct semver_s {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} semver;

#define ROSALIA_SEMVER_GET(prefix) \
    ((semver){ROSALIA_##prefix##_VERSION_MAJOR, ROSALIA_##prefix##_VERSION_MINOR, ROSALIA_##prefix##_VERSION_PATCH})

//TODO
// typedef struct semver_ext_s {
//     semver sv;
//     uint32_t release_c;
//     uint32_t build_c;
//     char** release_v;
//     char** build_v;
// } semver_ext;

// ROSALIA__SEMVER_DEC semver_ext ROSALIA__SEMVER_DECORATE(SEMVER_parse)(semver* sv, const char* str); //TODO
// ROSALIA__SEMVER_DEC semver_ext ROSALIA__SEMVER_DECORATE(SEMVER_parse_ext)(semver_ext* sve, const char* str); //TODO

//TODO some way to copy/free, preferably without serializer?

// ROSALIA__SEMVER_DEC size_t ROSALIA__SEMVER_DECORATE(SEMVER_size)(semver* sv); //TODO
// ROSALIA__SEMVER_DEC size_t ROSALIA__SEMVER_DECORATE(SEMVER_size_ext)(semver_ext* sv); //TODO

// ROSALIA__SEMVER_DEC size_t ROSALIA__SEMVER_DECORATE(SEMVER_print)(const char* str, semver* sv); //TODO
// ROSALIA__SEMVER_DEC size_t ROSALIA__SEMVER_DECORATE(SEMVER_print_ext)(const char* str, semver_ext* sv); //TODO

ROSALIA__SEMVER_DEC bool ROSALIA__SEMVER_DECORATE(SEMVER_equal)(semver l, semver r);

ROSALIA__SEMVER_DEC bool ROSALIA__SEMVER_DECORATE(SEMVER_satisfies)(semver required, semver provided);

#ifdef __cplusplus
}
#endif

#endif

//TODO serialization.h compatibility and semver+semver_ext serialization layout

#if defined(ROSALIA_SEMVER_IMPLEMENTATION) && !defined(ROSALIA_SEMVER_H_IMPL)
#define ROSALIA_SEMVER_H_IMPL

#define ROSALIA__SEMVER_INTERNAL(ident) rosalia__semver_internal_##ident

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__SEMVER_DEF bool ROSALIA__SEMVER_DECORATE(SEMVER_equal)(semver l, semver r)
{
    return l.major == r.major && l.minor == r.minor && l.patch == r.patch;
}

ROSALIA__SEMVER_DEF bool ROSALIA__SEMVER_DECORATE(SEMVER_satisfies)(semver required, semver provided)
{
    return provided.major == required.major && (provided.minor > required.minor || (provided.minor == required.minor && provided.patch >= required.patch));
}

#ifdef __cplusplus
}
#endif

#endif
