#pragma once
// provides very limited subset of semver

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ROSALIA_SEMVER_VERSION_MAJOR 0
#define ROSALIA_SEMVER_VERSION_MINOR 1
#define ROSALIA_SEMVER_VERSION_PATCH 5

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rosa_semver_s {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} rosa_semver;

static const rosa_semver rosalia_semver_version = (rosa_semver){0, 2, 0};

//TODO
// typedef struct rosa_semver_ext_s {
//     rosa_semver sv;
//     uint32_t release_c;
//     uint32_t build_c;
//     char** release_v;
//     char** build_v;
// } rosa_semver_ext;

// rosa_semver_ext SEMVER_parse(rosa_semver* sv, const char* str); //TODO
// rosa_semver_ext SEMVER_parse_ext(rosa_semver_ext* sve, const char* str); //TODO

//TODO some way to copy/free, preferably without serializer?

// size_t SEMVER_size(rosa_semver* sv); //TODO
// size_t SEMVER_size_ext(rosa_semver_ext* sv); //TODO

// size_t SEMVER_print(const char* str, rosa_semver* sv); //TODO
// size_t SEMVER_print_ext(const char* str, rosa_semver_ext* sv); //TODO

bool rosa_semver_equal(rosa_semver l, rosa_semver r);

bool rosa_semver_satisfies(rosa_semver required, rosa_semver provided);

#ifdef __cplusplus
}
#endif

//TODO serialization.h compatibility and semver+semver_ext serialization layout
