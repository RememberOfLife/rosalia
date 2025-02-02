#ifndef ROSALIA_TREEMAP_H_INCLUDE
#define ROSALIA_TREEMAP_H_INCLUDE

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef ROSALIA_TREEMAP_STATIC
#define ROSALIA__TREEMAP_DEC static
#define ROSALIA__TREEMAP_DEC_EXT static
#define ROSALIA__TREEMAP_DEF static
#else
#define ROSALIA__TREEMAP_DEC
#define ROSALIA__TREEMAP_DEC_EXT extern
#define ROSALIA__TREEMAP_DEF
#endif

#define ROSALIA_TREEMAP_VERSION_MAJOR 0
#define ROSALIA_TREEMAP_VERSION_MINOR 0
#define ROSALIA_TREEMAP_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ROSALIA_TREEMAP_NO_SHORT_NAMES
#define TREEMAP ROSA_TREEMAP
#define TREESET ROSA_TREESET
//TODO
#endif

//TODO

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_TREEMAP_IMPLEMENTATION) && !defined(ROSALIA_TREEMAP_H_IMPL)
#define ROSALIA_TREEMAP_H_IMPL

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
