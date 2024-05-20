#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO this is just remnants, remove soon
// for concurrent access to the tree, later on might make the tree more capable and lock free
void* cfg_lock_create();
void cfg_lock_destroy(void* lock);
void cfg_rlock(void* lock);
void cfg_runlock(void* lock);
void cfg_wlock(void* lock);
void cfg_wunlock(void* lock);

#ifdef __cplusplus
}
#endif
