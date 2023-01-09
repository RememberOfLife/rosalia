#ifndef ROSALIA_SEMVER_H_INCLUDE
#define ROSALIA_SEMVER_H_INCLUDE

// provides very limited subset of semver

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ROSALIA_SEMVER_STATIC
#define ROSALIA__DEC static
#define ROSALIA__DEF static
#else
#define ROSALIA__DEC
#define ROSALIA__DEF
#endif

#ifdef ROSALIA_SEMVER_DECORATE
#define ROSALIA__DECORATE(ident) ROSALIA_SEMVER_DECORATE(ident)
#else
#define ROSALIA__DECORATE(ident) ident
#endif

#define ROSALIA_SEMVER_VERSION_MAJOR 0
#define ROSALIA_SEMVER_VERSION_MINOR 1
#define RSOALIA_SEMVER_VERSION_PATCH 0

typedef struct semver_s {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} semver;

//TODO semver_ext with pre-release and build info tags

#define ROSALIA_SEMVER_GET(prefix) \
    ((semver){ROSALIA_##prefix##_VERSION_MAJOR, ROSALIA_##prefix##_VERSION_MINOR, ROSALIA_##prefix##_VERSION_PATCH})

ROSALIA__DEC bool ROSALIA__DECORATE(SEMVER_equal)(semver l, semver r);

ROSALIA__DEC bool ROSALIA__DECORATE(SEMVER_satisfies)(semver required, semver provided);

#ifdef __cplusplus
}
#endif

#endif

#ifdef ROSALIA_SEMVER_IMPLEMENTATION

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
#endif

#undef ROSALIA__DEC
#undef ROSALIA__DEF
#undef ROSALIA__DECORATE
