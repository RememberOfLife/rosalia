#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// https://github.com/nothings/stb/blob/master/stb_leakcheck.h

//TODO ability to use custom context

//TODO provide memcheck composition part for allocator

/*
* SM: debug utility for malloc free replace to find leaks etc
  * use pthreadmutex
  * on alloc place cannaries to detect byte exact boundary violations
    * seeded randomly on startup
  * on free, do not actually free, but hash the value of the userdata, place it with the allocinfo and mark as deleted
    * provide some mem_consistency check function that rehashed all userdata and compares it
      * i.e. reports illegal use after free
    * report double frees
*/

//TODO want rosalia/memcheck_enable.h for this specifically?
#ifdef ROSA_MEMCHECK_ENABLE
#define malloc(s) rosa_memcheck_malloc(s, __FILE__, __LINE__)
#define free(p) rosa_memcheck_free(p)
#endif

typedef struct rosa_memcheck_entry_s rosa_memcheck_entry;

struct rosa_memcheck_entry_s {
    rosa_memcheck_entry* next;
    rosa_memcheck_entry* prev;
    const char* file_name;
    int file_line;
    size_t size;
};

extern rosa_memcheck_entry* rosa_memcheck_entries;

void* rosa_memcheck_malloc(size_t s, const char* file, int line);

void rosa_memcheck_free(void* p);

void rosa_memcheck_info();

void rosa_memcheck_integrity();

#ifdef __cplusplus
}
#endif
