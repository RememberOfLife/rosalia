#ifndef ROSALIA_UTIL_H_INCLUDE
#define ROSALIA_UTIL_H_INCLUDE

#include <stdbool.h>

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
#define ROSALIA_UTIL_VERSION_PATCH 1

#ifdef __cplusplus
extern "C" {
#endif

#ifndef offsetof
#define offsetof(st, m) \
    ((size_t)((char*)&((st*)0)->m - (char*)0))
#endif

#define container_of(ptr, type, member) ((type*)((char*)(1 ? (ptr) : &((type*)0)->member) - offsetof(type, member)))

#define CATI(x, y) x##y
#define MCAT(x, y) CATI(x, y)

//TODO ptradd should go here

#ifndef NDEBUG
#define unreachable panic_unreachable_reached(__FILE__, __LINE__)
#else
#define unreachable panic_unreachable_reached("<src-unavailable>", __COUNTER__)
#endif

void panic_unreachable_reached(const char* file, int line);

#ifndef NDEBUG
#define rtassert(cond, msg) panic_rtassert_failed((cond), (msg), __FILE__, __LINE__)
#else
#define rtassert(cond, msg) panic_rtassert_failed((cond), (msg), "<src-unavailable>", __COUNTER__)
#endif

void panic_rtassert_failed(bool cond, const char* msg, const char* file, int line);

#ifdef __cplusplus
}
#endif

#endif

#if defined(ROSALIA_UTIL_IMPLEMENTATION) && !defined(ROSALIA_UTIL_H_IMPL)
#define ROSALIA_UTIL_H_IMPL

#include <stdio.h>
#include <stdlib.h>

//TODO use log callback instead of fprintf

void panic_unreachable_reached(const char* file, int line)
{
    // log_cb(LOGS_FATAL, "unreachable code reached: %s:%i\n", file, line);
    fprintf(stderr, "unreachable code reached: %s:%i\n", file, line);
    exit(1);
}

void panic_rtassert_failed(bool cond, const char* msg, const char* file, int line)
{
    if (cond) {
        return;
    }
    fprintf(stderr, "assert failed: %s:%i%s%s\n", file, line, msg != NULL ? " --: " : "", msg != NULL ? msg : "");
    exit(1);
}

#endif
