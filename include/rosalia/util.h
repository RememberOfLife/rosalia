#pragma once

#include <stdbool.h>

#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

static const rosa_semver rosalia_util_version = (rosa_semver){0, 2, 0};

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
