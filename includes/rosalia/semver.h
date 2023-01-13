#ifndef ROSALIA_SEMVER_H_INCLUDE
#define ROSALIA_SEMVER_H_INCLUDE

// provides very limited subset of semver

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_SEMVER_STATIC
#define ROSALIA__DEC static
#define ROSALIA__DEC_EXT static
#define ROSALIA__DEF static
#else
#define ROSALIA__DEC
#define ROSALIA__DEC_EXT extern
#define ROSALIA__DEF
#endif

#ifdef ROSALIA_SEMVER_DECORATE
#define ROSALIA__DECORATE(ident) ROSALIA_SEMVER_DECORATE(ident)
#else
#define ROSALIA__DECORATE(ident) ident
#endif

#define ROSALIA_SEMVER_VERSION_MAJOR 0
#define ROSALIA_SEMVER_VERSION_MINOR 1
#define ROSALIA_SEMVER_VERSION_PATCH 1

#ifdef __cplusplus
extern "C" {
#endif

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

// ROSALIA__DEC semver_ext ROSALIA__DECORATE(SEMVER_parse)(semver* sv, const char* str); //TODO
// ROSALIA__DEC semver_ext ROSALIA__DECORATE(SEMVER_parse_ext)(semver_ext* sve, const char* str); //TODO

//TODO some way to copy/free, preferably without serializer?

// ROSALIA__DEC size_t ROSALIA__DECORATE(SEMVER_size)(semver* sv); //TODO
// ROSALIA__DEC size_t ROSALIA__DECORATE(SEMVER_size_ext)(semver_ext* sv); //TODO

// ROSALIA__DEC size_t ROSALIA__DECORATE(SEMVER_print)(const char* str, semver* sv); //TODO
// ROSALIA__DEC size_t ROSALIA__DECORATE(SEMVER_print_ext)(const char* str, semver_ext* sv); //TODO

ROSALIA__DEC bool ROSALIA__DECORATE(SEMVER_equal)(semver l, semver r);

ROSALIA__DEC bool ROSALIA__DECORATE(SEMVER_satisfies)(semver required, semver provided);

#ifdef __cplusplus
}
#endif

#endif

//TODO serialization.h compatibility and semver+semver_ext serialization layout

#ifdef ROSALIA_SEMVER_IMPLEMENTATION

#define ROSALIA__INTERNAL(ident) rosalia__internal_##ident

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

ROSALIA__DEF bool ROSALIA__DECORATE(SEMVER_equal)(semver l, semver r)
{
    return l.major == r.major && l.minor == r.minor && l.patch == r.patch;
}

ROSALIA__DEF bool ROSALIA__DECORATE(SEMVER_satisfies)(semver required, semver provided)
{
    return provided.major == required.major && (provided.minor > required.minor || (provided.minor == required.minor && provided.patch >= required.patch));
}

#ifdef __cplusplus
}
#endif

#undef ROSALIA_SEMVER_IMPLEMENTATION
#undef ROSALIA__INTERNAL
#endif

#undef ROSALIA__DEC
#undef ROSALIA__DEC_EXT
#undef ROSALIA__DEF
#undef ROSALIA__DECORATE
