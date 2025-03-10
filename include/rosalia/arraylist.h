#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

//TODO test the whole thing and adapt for new prototype layouts.. this will change a lot, and RENAME for actual arraylist names

static const rosa_semver rosalia_arraylist_version = (rosa_semver){0, 5, 1};

// typed vector macro api taken from: https://github.com/nothings/stb/blob/master/stb_ds.h in the public domain
// api is slightly adapted, make sure to read each the macros usage instructions after the short names

//TODO determine how I want this to work
#ifdef ROSALIA__DROP_PREFIX
#define ARRAYLIST ROSA_ARRAYLIST
#define ARRAYLIST_CREATE ROSA_ARRAYLIST_CREATE
#define ARRAYLIST_DESTROY ROSA_ARRAYLIST_DESTROY
#define ARRAYLIST_CAP ROSA_ARRAYLIST_CAP
#define ARRAYLIST_SETCAP ROSA_ARRAYLIST_SETCAP
#define ARRAYLIST_FITCAP ROSA_ARRAYLIST_FITCAP
#define ARRAYLIST_LEN ROSA_ARRAYLIST_LEN
#define ARRAYLIST_SETLEN ROSA_ARRAYLIST_SETLEN
#define ARRAYLIST_FITLEN ROSA_ARRAYLIST_FITLEN
#define ARRAYLIST_ADDLEN_IDX ROSA_ARRAYLIST_ADDLEN_IDX
#define ARRAYLIST_ADDLEN_PTR ROSA_ARRAYLIST_ADDLEN_PTR
#define ARRAYLIST_PUSH ROSA_ARRAYLIST_PUSH
#define ARRAYLIST_PUSH_N ROSA_ARRAYLIST_PUSH_N
#define ARRAYLIST_POP ROSA_ARRAYLIST_POP
#define ARRAYLIST_POP_N ROSA_ARRAYLIST_POP_N
#define ARRAYLIST_PEEK ROSA_ARRAYLIST_PEEK
#define ARRAYLIST_INSERT ROSA_ARRAYLIST_INSERT
#define ARRAYLIST_INSERT_N ROSA_ARRAYLIST_INSERT_N
#define ARRAYLIST_REMOVE ROSA_ARRAYLIST_REMOVE
#define ARRAYLIST_REMOVE_N ROSA_ARRAYLIST_REMOVE_N
#define ARRAYLIST_REMOVE_SWAP ROSA_ARRAYLIST_REMOVE_SWAP
#define ARRAYLIST_REMOVE_SWAP_N ROSA_ARRAYLIST_REMOVE_SWAP_N
#define ARRAYLIST_LAST ROSA_ARRAYLIST_LAST
#define ARRAYLIST_CLONE ROSA_ARRAYLIST_CLONE
#endif

// short names enabled by default
#ifndef ROSALIA_ARRAYLIST_NO_SHORT_NAMES
#define ALIST ROSA_ARRAYLIST
#define ALIST_CREATE ROSA_ARRAYLIST_CREATE
#define ALIST_DESTROY ROSA_ARRAYLIST_DESTROY
#define ALIST_CAP ROSA_ARRAYLIST_CAP
#define ALIST_SETCAP ROSA_ARRAYLIST_SETCAP
#define ALIST_FITCAP ROSA_ARRAYLIST_FITCAP
#define ALIST_LEN ROSA_ARRAYLIST_LEN
#define ALIST_SETLEN ROSA_ARRAYLIST_SETLEN
#define ALIST_FITLEN ROSA_ARRAYLIST_FITLEN
#define ALIST_ADDLEN_IDX ROSA_ARRAYLIST_ADDLEN_IDX
#define ALIST_ADDLEN_PTR ROSA_ARRAYLIST_ADDLEN_PTR
#define ALIST_PUSH ROSA_ARRAYLIST_PUSH
#define ALIST_PUSH_N ROSA_ARRAYLIST_PUSH_N
#define ALIST_POP ROSA_ARRAYLIST_POP
#define ALIST_POP_N ROSA_ARRAYLIST_POP_N
#define ALIST_PEEK ROSA_ARRAYLIST_PEEK
#define ALIST_INSERT ROSA_ARRAYLIST_INSERT
#define ALIST_INSERT_N ROSA_ARRAYLIST_INSERT_N
#define ALIST_REMOVE ROSA_ARRAYLIST_REMOVE
#define ALIST_REMOVE_N ROSA_ARRAYLIST_REMOVE_N
#define ALIST_REMOVE_SWAP ROSA_ARRAYLIST_REMOVE_SWAP
#define ALIST_REMOVE_SWAP_N ROSA_ARRAYLIST_REMOVE_SWAP_N
#define ALIST_LAST ROSA_ARRAYLIST_LAST
#define ALIST_CLONE ROSA_ARRAYLIST_CLONE
#endif

//TODO alloc.h compat

//TODO check all of these for correct behaviour
//TODO e.g. things like push n with len 0 should not fail on NULL arraylists

typedef struct rosa__arraylist_info_s {
    size_t length;
    size_t capacity;
} rosa__arraylist_info;

#define ROSA__ARRAYLIST_GET_INFO(pp_vec) (((rosa__arraylist_info*)*(pp_vec)) - 1)

// takes a vector and returns a vector that:
// has at least enough space to fit additional add_len elements and has a minimum capacity of min_cap
void* rosalia__arraylist_grow_impl(void* p_vec, size_t elem_size, size_t add_len, size_t min_cap);

// takes a vector and returns a vector that:
// has enough space for fit many elements, unused capacity is released
void* rosa__arraylist_shrink_to_fit(void* p_vec, size_t elem_size, size_t fit);

#define ROSA__ARRAYLIST_GROW(pp_vec, len, cap) (*((void**)(pp_vec)) = rosalia__arraylist_grow_impl(*(pp_vec), sizeof(**(pp_vec)), (len), (cap)))

#define ROSA__ARRAYLIST_GROW_MIN_LEN(pp_vec, len) ((*(pp_vec) == NULL || ROSA__ARRAYLIST_GET_INFO(pp_vec)->length + (len) > ROSA__ARRAYLIST_GET_INFO(pp_vec)->capacity) ? (ROSA__ARRAYLIST_GROW((pp_vec), (len), 0), 0) : 0)

//////
// for an arraylist of type T
// ROSA_ARRAYLIST(T) my_list;
// or, discouraged
// T* my_list = NULL;

#define ROSA_ARRAYLIST(type) type*

// create(T** pp_vec, size_t cap)
// allocate the vector with an initial capacity of cap
#define ROSA_ARRAYLIST_CREATE(pp_vec, cap) (*(pp_vec) = NULL, ROSA__ARRAYLIST_GROW((pp_vec), 0, (cap)))

// aesthetic macro
// destroy(T** pp_vec)
// free the vector and set it to NULL
#define ROSA_ARRAYLIST_DESTROY(pp_vec) ((void)(*(pp_vec) != NULL ? free(ROSA__ARRAYLIST_GET_INFO(pp_vec)) : (void)0), *(pp_vec) = NULL)

// aesthetic macro
// size_t cap(T** pp_vec)
// returns the capacity of the vector, i.e. num of elems it could contain before it must be reallocated to grow
#define ROSA_ARRAYLIST_CAP(pp_vec) (*(pp_vec) != NULL ? ROSA__ARRAYLIST_GET_INFO(pp_vec)->capacity : 0)

// setcap(T** pp_vec, size_t cap)
// set the capacity of the vector, this will grow the vector if required
// note that the capacity may also be lower than the length, in this cas
#define ROSA_ARRAYLIST_SETCAP(pp_vec, cap) (ROSA__ARRAYLIST_GROW((pp_vec), 0, (cap)))

// fitcap
// shrink to fit capacity:
// len < cap : "expected natural shrink", fit=max(len*2,4)
// len == cap : realloc to fit=len //TODO might want natural shrink here too
// len > cap : truncate length, realloc to fit=cap
#define ROSA_ARRAYLIST_FITCAP(pp_vec) (*((void**)pp_vec) = rosa__arraylist_shrink_to_fit(*(pp_vec), sizeof(**(pp_vec)), ROSA_ARRAYLIST_LEN(pp_vec) < ROSA_ARRAYLIST_CAP(pp_vec) ? 2 * ROSA_ARRAYLIST_LEN(pp_vec) : ROSA_ARRAYLIST_CAP(pp_vec)))

// aesthetic macro
// size_t len(T** pp_vec)
// returns the number of elements currently in the vector
#define ROSA_ARRAYLIST_LEN(pp_vec) (*(pp_vec) != NULL ? ROSA__ARRAYLIST_GET_INFO(pp_vec)->length : 0)

// setlen(T** pp_vec, size_t len)
// set the length of the vector, this will add additional uninitialized elements at the end if necessary
#define ROSA_ARRAYLIST_SETLEN(pp_vec, len) ((ROSA_ARRAYLIST_CAP(pp_vec) < (size_t)(len) ? ROSA_ARRAYLIST_SETCAP((pp_vec), (size_t)(len)), 0 : 0), *(pp_vec) != NULL ? ROSA__ARRAYLIST_GET_INFO(pp_vec)->length = (size_t)(len) : 0)

// fitlen(T** pp_vec)
// shrink to fit length, this is the strictest shrinking
// note a vector will never shrink below 4 elements
#define ROSA_ARRAYLIST_FITLEN(pp_vec) (*((void**)pp_vec) = rosa__arraylist_shrink_to_fit(*(pp_vec), sizeof(**(pp_vec)), ROSA_ARRAYLIST_LEN(pp_vec)))

// addlen_idx(T** pp_vec, size_t len)
// add len uninitialized elements to the end of the vector and return the index of the first of them
#define ROSA_ARRAYLIST_ADDLEN_IDX(pp_vec, len) (ROSA__ARRAYLIST_GROW_MIN_LEN((pp_vec), (len)), (len) > 0 ? (ROSA__ARRAYLIST_GET_INFO(pp_vec)->length += (n), ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - (n)) : ROSA_ARRAYLIST_LEN(pp_vec))

// addlen_ptr(T** pp_vec, size_t len)
// add len uninitialized elements to the end of the vector and return a ptr to the first of them
#define ROSA_ARRAYLIST_ADDLEN_PTR(pp_vec, len) (ROSA__ARRAYLIST_GROW_MIN_LEN((pp_vec), (len)), (len) > 0 ? (ROSA__ARRAYLIST_GET_INFO(pp_vec)->length += (len), &(*(pp_vec))[ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - (len)]) : *(pp_vec))

// push(T** pp_vec, T elem)
// add elem to the end of the vector
#define ROSA_ARRAYLIST_PUSH(pp_vec, elem) (ROSA__ARRAYLIST_GROW_MIN_LEN((pp_vec), 1), (*(pp_vec))[ROSA__ARRAYLIST_GET_INFO(pp_vec)->length++] = (elem))

// push_n(T** pp_vec, size_t len)
// add len uninitialized elements to the end of the vector
#define ROSA_ARRAYLIST_PUSH_N(pp_vec, len) (ROSA__ARRAYLIST_GROW_MIN_LEN((pp_vec), (len)), ROSA__ARRAYLIST_GET_INFO(pp_vec)->length += len)

// T pop(T** pp_vec)
// get the last element of the vector, and remove it at the same time
#define ROSA_ARRAYLIST_POP(pp_vec) (ROSA__ARRAYLIST_GET_INFO(pp_vec)->length--, (*(pp_vec))[ROSA__ARRAYLIST_GET_INFO(pp_vec)->length])

// pop_n(T** pp_vec, size_t len)
// remove the len last elements from the vector
#define ROSA_ARRAYLIST_POP_N(pp_vec, len) (ROSA__ARRAYLIST_GET_INFO(pp_vec)->length >= len ? ROSA__ARRAYLIST_GET_INFO(pp_vec)->length -= len : ROSA__ARRAYLIST_GET_INFO(pp_vec)->length = 0)

// T peek(T** pp_vec, size_t ridx)
// get the element at ridx from the end of the vector, i.e. vec[VEC_LEN(&vec) - 1 - ridx]
#define ROSA_ARRAYLIST_PEEK(pp_vec, ridx) ((*(pp_vec))[ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - 1 - (rdix)])

// insert(T** pp_vec, size_t idx, T elem)
// insert elem T such that it is reachable at idx
#define ROSA_ARRAYLIST_INSERT(pp_vec, idx, elem) ((ROSA_ARRAYLIST_INSERT_N((pp_vec), (idx), 1), (*(pp_vec))[idx] = (elem)))

// insert_n(T** pp_vec, size_t idx, size_t len)
// inserts len uninitialized elements at idx forward
#define ROSA_ARRAYLIST_INSERT_N(pp_vec, idx, len) (ROSA_ARRAYLIST_ADDLEN((pp_vec), (len)), memmove(&(*(pp_vec))[(idx) + (len)], &(*(pp_vec))[idx], sizeof(**(pp_vec)) * (ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - (len) - (idx))))

// remove(T** pp_vec, size_t idx)
// remove element at idx and copy the rest so the order stays stable
#define ROSA_ARRAYLIST_REMOVE(pp_vec, idx) (ROSA_ARRAYLIST_REMOVE_N((pp_vec), (idx), 1))

// remove_n(T** pp_vec, size_t idx, size_t len)
// remove len elements at idx forward and copy the rest so the order stays stable
#define ROSA_ARRAYLIST_REMOVE_N(pp_vec, idx, len) (memmove(&(*(pp_vec))[idx], &(*(pp_vec))[(idx) + (len)], sizeof(**(pp_vec)) * (ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - (len) - (idx))), ROSA__ARRAYLIST_GET_INFO(pp_vec)->length -= (len))

// remove_swap(T** pp_vec, size_t idx)
// remove element at idx and swap in an element from the back, order is not stable, but is much faster than copying
#define ROSA_ARRAYLIST_REMOVE_SWAP(pp_vec, idx) (ROSA_ARRAYLIST_REMOVE_SWAP_N((pp_vec), (idx), 1))

// remove_swap_n(T** pp_vec, size_t idx, size_t len)
// remove len elements at idx forward and swap in element from the back (if required), order is not stable, but is much faster than copying
#define ROSA_ARRAYLIST_REMOVE_SWAP_N(pp_vec, idx, len) (memmove(&(*(pp_vec))[idx], &(*(pp_vec))[ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - ((idx) + (len) > ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - (len) ? ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - (idx) - (len) : (len))], sizeof(**(pp_vec)) * ((idx) + (len) > ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - (len) ? ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - (idx) - (len) : (len))), ROSA__ARRAYLIST_GET_INFO(pp_vec)->length -= (len))

// T last(T** pp_vec)
// get the last element in the vector
#define ROSA_ARRAYLIST_LAST(pp_vec) ((*(pp_vec))[ROSA__ARRAYLIST_GET_INFO(pp_vec)->length - 1])

// clone(T** pp_vec_destination, T** pp_vec_source)
// shallow clones source vector into destination, if source is NULL or empty, then so will the destination be afterwards
#define ROSA_ARRAYLIST_CLONE(pp_vec_d, pp_vec_s) (ROSA_ARRAYLIST_CREATE((pp_vec_d), ROSA_ARRAYLIST_LEN(pp_vec_s)), *(pp_vec_s) != NULL ? (ROSA__ARRAYLIST_GET_INFO(pp_vec_d)->length = ROSA__ARRAYLIST_GET_INFO(pp_vec_s)->length, memcpy(*(pp_vec_d), *(pp_vec_s), sizeof(**(pp_vec_s)) * ROSA__ARRAYLIST_GET_INFO(pp_vec_s)->length), 0) : (*(pp_vec_d) = NULL, 0))

#ifdef __cplusplus
}
#endif
