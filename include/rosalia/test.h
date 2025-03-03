#pragma once

#include <sys/mman.h>

#include "rosalia/arraylist.h"

#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

static const rosa_semver rosalia_test_version = (rosa_semver){0, 1, 0};

typedef struct rosa_tester_s rosa_tester;

typedef void rosa_test_t(rosa_tester* t, size_t test_nr, size_t test_case);

typedef struct rosa_tester_test_case_s {
    const char* file;
    int line;
    const char* name;
} rosa_tester_test_case;

static const size_t test_case_none = SIZE_MAX;
static const size_t test_case_base_none = SIZE_MAX;

typedef struct rosa_tester_test_s {
    const char* file;
    int line;
    const char* name;
    rosa_test_t* func;
    size_t cases_base;
    size_t case_count;
    size_t current_case_ctr;
} rosa_tester_test;

struct rosa_tester_s {
    size_t max_tests;
    rosa_tester_test* tests;
    size_t test_count;
    size_t max_cases;
    rosa_tester_test_case* cases;
    size_t case_count;
};

//TODO some kind of arguments
// returns true on success
bool rosa_tester_create(rosa_tester* t, size_t max_tests, size_t max_cases);

void rosa__tester_add_impl(rosa_tester* t, rosa_test_t* test_func, const char* test_name, const char* file, int line);

#define ROSA_TESTER_ADD(p_tester, test_func) rosa__tester_add_impl(p_tester, test_##test_func, #test_func, __FILE__, __LINE__)

size_t rosa_tester_run(rosa_tester* t);

void rosa_tester_destroy(rosa_tester* t);

#define ROSA_TEST(ident) void test_##ident(rosa_tester* t, size_t test_nr, size_t test_case)

bool rosa__tester_test_case(rosa_tester* t, size_t test_nr, size_t test_case, const char* test_case_name, const char* file, int line);

#define ROSA_TEST_CASE(test_case_name) \
    if (rosa__tester_test_case(t, test_nr, test_case, test_case_name, __FILE__, __LINE__))

//TODO want to offer a tassert for asserting while testing?
/*
#define tassert0(cond)

#define tassert1(cond, msg)

//TODO this would go into utils likely..
#define GET_MACRO_0_1(_1, _2, name, ...) name

#define tassert(...) GET_MACRO_0_1(__VA_ARGS__, tassert1, tassert0)(__VA_ARGS__)
*/

#ifdef __cplusplus
}
#endif
