#ifndef ROSALIA_UTIL_H_INCLUDE
#define ROSALIA_UTIL_H_INCLUDE

#ifdef ROSALIA_UTIL_STATIC
#define ROSALIA__UTIL_DEC static
#define ROSALIA__UTIL_DEC_EXT static
#define ROSALIA__UTIL_DEF static
#else
#define ROSALIA__UTIL_DEC
#define ROSALIA__UTIL_DEC_EXT extern
#define ROSALIA__UTIL_DEF
#endif

#define ROSALIA_UTIL_VERSION_MAJOR 0
#define ROSALIA_UTIL_VERSION_MINOR 1
#define ROSALIA_UTIL_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

#ifndef offsetof
#define offsetof(st, m) \
    ((size_t)((char*)&((st*)0)->m - (char*)0))
#endif

#define container_of(ptr, type, member) ((type*)((char*)(1 ? (ptr) : &((type*)0)->member) - offsetof(type, member)))

//TODO ptradd should go here

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_UTIL_IMPLEMENTATION) && !defined(ROSALIA_UTIL_H_IMPL)
#define ROSALIA_UTIL_H_IMPL

#endif
