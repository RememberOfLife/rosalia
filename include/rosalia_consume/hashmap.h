#ifndef ROSALIA_HASHMAP_H_INCLUDE
#define ROSALIA_HASHMAP_H_INCLUDE

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef ROSALIA_HASHMAP_STATIC
#define ROSALIA__HASHMAP_DEC static
#define ROSALIA__HASHMAP_DEC_EXT static
#define ROSALIA__HASHMAP_DEF static
#else
#define ROSALIA__HASHMAP_DEC
#define ROSALIA__HASHMAP_DEC_EXT extern
#define ROSALIA__HASHMAP_DEF
#endif

#define ROSALIA_HASHMAP_VERSION_MAJOR 0
#define ROSALIA_HASHMAP_VERSION_MINOR 0
#define ROSALIA_HASHMAP_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ROSALIA_HASHMAP_NO_SHORT_NAMES
#define HASHMAP ROSA_HASHMAP
#define HASHSET ROSA_HASHSET
//TODO
#endif

//TODO

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_HASHMAP_IMPLEMENTATION) && !defined(ROSALIA_HASHMAP_H_IMPL)
#define ROSALIA_HASHMAP_H_IMPL

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO

#ifdef __cplusplus
}
#endif

#endif
