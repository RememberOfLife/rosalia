#ifndef ROSALIA_LINKLIST_H_INCLUDE
#define ROSALIA_LINKLIST_H_INCLUDE

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef ROSALIA_LINKLIST_STATIC
#define ROSALIA__LINKLIST_DEC static
#define ROSALIA__LINKLIST_DEC_EXT static
#define ROSALIA__LINKLIST_DEF static
#else
#define ROSALIA__LINKLIST_DEC
#define ROSALIA__LINKLIST_DEC_EXT extern
#define ROSALIA__LINKLIST_DEF
#endif

#define ROSALIA_LINKLIST_VERSION_MAJOR 0
#define ROSALIA_LINKLIST_VERSION_MINOR 0
#define ROSALIA_LINKLIST_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ROSALIA_LINKLIST_NO_SHORT_NAMES
#define LINKLIST ROSA_LINKLIST
//TODO
#endif

//TODO

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_LINKLIST_IMPLEMENTATION) && !defined(ROSALIA_LINKLIST_H_IMPL)
#define ROSALIA_LINKLIST_H_IMPL

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
