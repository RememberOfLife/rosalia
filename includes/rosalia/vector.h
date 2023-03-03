#ifndef ROSALIA_VECTOR_H_INCLUDE
#define ROSALIA_VECTOR_H_INCLUDE

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef ROSALIA_VECTOR_STATIC
#define ROSALIA__VECTOR_DEC static
#define ROSALIA__VECTOR_DEC_EXT static
#define ROSALIA__VECTOR_DEF static
#else
#define ROSALIA__VECTOR_DEC
#define ROSALIA__VECTOR_DEC_EXT extern
#define ROSALIA__VECTOR_DEF
#endif

#ifdef ROSALIA_VECTOR_DECORATE
#define ROSALIA__VECTOR_DECORATE(ident) ROSALIA_VECTOR_DECORATE(ident)
#else
#define ROSALIA__VECTOR_DECORATE(ident) ident
#endif

#define ROSALIA_VECTOR_VERSION_MAJOR 0
#define ROSALIA_VECTOR_VERSION_MINOR 4
#define ROSALIA_VECTOR_VERSION_PATCH 6

#ifdef __cplusplus
extern "C" {
#endif

// typed vector macro api taken from: https://github.com/nothings/stb/blob/master/stb_ds.h in the public domain
// api is slightly adapted, make sure to read each the macros usage instructions after the short names

// short names enabled by default
#ifndef ROSALIA_VECTOR_NO_SHORT_NAMES
#define VEC_CREATE ROSALIA_VECTOR_CREATE
#define VEC_DESTROY ROSALIA_VECTOR_DESTROY // this one doesn't need to be a macro
#define VEC_CAP ROSALIA_VECTOR_CAP // this one doesn't need to be a macro
#define VEC_SETCAP ROSALIA_VECTOR_SETCAP
#define VEC_FITCAP ROSALIA_VECTOR_FITCAP
#define VEC_LEN ROSALIA_VECTOR_LEN // this one doesn't need to be a macro
#define VEC_SETLEN ROSALIA_VECTOR_SETLEN
#define VEC_FITLEN ROSALIA_VECTOR_FITLEN
#define VEC_ADDLEN_IDX ROSALIA_VECTOR_ADDLEN_IDX
#define VEC_ADDLEN_PTR ROSALIA_VECTOR_ADDLEN_PTR
#define VEC_PUSH ROSALIA_VECTOR_PUSH
#define VEC_PUSH_N ROSALIA_VECTOR_PUSH_N
#define VEC_POP ROSALIA_VECTOR_POP
#define VEC_POP_N ROSALIA_VECTOR_POP_N
#define VEC_PEEK ROSALIA_VECTOR_PEEK
#define VEC_INSERT ROSALIA_VECTOR_INSERT
#define VEC_INSERT_N ROSALIA_VECTOR_INSERT_N
#define VEC_REMOVE ROSALIA_VECTOR_REMOVE
#define VEC_REMOVE_N ROSALIA_VECTOR_REMOVE_N
#define VEC_REMOVE_SWAP ROSALIA_VECTOR_REMOVE_SWAP
#define VEC_REMOVE_SWAP_N ROSALIA_VECTOR_REMOVE_SWAP_N
#define VEC_LAST ROSALIA_VECTOR_LAST
#define VEC_CLONE ROSALIA_VECTOR_CLONE
#endif

//TODO alloc.h compat

//TODO check all of these for correct behaviour

//TODO things like push n with len 0 should not fail on NULL vectors

typedef struct rosalia__internal_vector_info_s {
    size_t length;
    size_t capacity;
} rosalia__internal_vector_info;

#define ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec) (((rosalia__internal_vector_info*)*(pp_vec)) - 1)

// takes a vector and returns a vector that:
// has at least enough space to fit additional add_len elements and has a minimum capacity of min_cap
ROSALIA__VECTOR_DEC void* rosalia__vector_internal_grow(void* p_vec, size_t elem_size, size_t add_len, size_t min_cap);

// takes a vector and returns a vector that:
// has enough space for fit many elements, unused capacity is released
ROSALIA__VECTOR_DEC void* rosalia__vector_internal_shrink_to_fit(void* p_vec, size_t elem_size, size_t fit);

#define ROSALIA__VECTOR_INTERNAL_VECTOR_GROW(pp_vec, len, cap) (*((void**)(pp_vec)) = rosalia__vector_internal_grow(*(pp_vec), sizeof(**(pp_vec)), (len), (cap)))

#define ROSALIA__VECTOR_INTERNAL_VECTOR_MGROW(pp_vec, len) ((*(pp_vec) == NULL || ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length + (len) > ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->capacity) ? (ROSALIA__VECTOR_INTERNAL_VECTOR_GROW((pp_vec), (len), 0), 0) : 0)

//////
// for a vector of type T
// T* myvec = NULL;

// create(T** pp_vec, size_t cap)
// allocate the vector with an initial capacity of cap
#define ROSALIA_VECTOR_CREATE(pp_vec, cap) (*(pp_vec) = NULL, ROSALIA__VECTOR_INTERNAL_VECTOR_GROW((pp_vec), 0, (cap)))

// destroy(T** pp_vec)
// free the vector and set it to NULL
#define ROSALIA_VECTOR_DESTROY(pp_vec) ((void)(*(pp_vec) != NULL ? free(ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)) : (void)0), *(pp_vec) = NULL)

// size_t cap(T** pp_vec)
// returns the capacity of the vector, i.e. num of elems it could contain before it must be reallocated to grow
#define ROSALIA_VECTOR_CAP(pp_vec) (*(pp_vec) != NULL ? ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->capacity : 0)

// setcap(T** pp_vec, size_t cap)
// set the capacity of the vector, this will grow the vector if required
// note that the capacity may also be lower than the length, in this cas
#define ROSALIA_VECTOR_SETCAP(pp_vec, cap) (ROSALIA__VECTOR_INTERNAL_VECTOR_GROW((pp_vec), 0, (cap)))

// fitcap
// shrink to fit capacity:
// len < cap : "expected natural shrink", fit=max(len*2,4)
// len == cap : realloc to fit=len //TODO might want natural shrink here too
// len > cap : truncate length, realloc to fit=cap
#define ROSALIA_VECTOR_FITCAP(pp_vec) (*((void**)pp_vec) = rosalia__vector_internal_shrink_to_fit(*(pp_vec), sizeof(**(pp_vec)), ROSALIA_VECTOR_LEN(pp_vec) < ROSALIA_VECTOR_CAP(pp_vec) ? 2 * ROSALIA_VECTOR_LEN(pp_vec) : ROSALIA_VECTOR_CAP(pp_vec)))

// size_t len(T** pp_vec)
// returns the number of elements currently in the vector
#define ROSALIA_VECTOR_LEN(pp_vec) (*(pp_vec) != NULL ? ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length : 0)

// setlen(T** pp_vec, size_t len)
// set the length of the vector, this will add additional uninitialized elements at the end if necessary
#define ROSALIA_VECTOR_SETLEN(pp_vec, len) ((ROSALIA_VECTOR_CAP(pp_vec) < (size_t)(len) ? ROSALIA_VECTOR_SETCAP((pp_vec), (size_t)(len)), 0 : 0), *(pp_vec) != NULL ? ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length = (size_t)(len) : 0)

// fitlen(T** pp_vec)
// shrink to fit length, this is the strictest shrinking
// note a vector will never shrink below 4 elements
#define ROSALIA_VECTOR_FITLEN(pp_vec) (*((void**)pp_vec) = rosalia__vector_internal_shrink_to_fit(*(pp_vec), sizeof(**(pp_vec)), ROSALIA_VECTOR_LEN(pp_vec)))

// addlen_idx(T** pp_vec, size_t len)
// add len uninitialized elements to the end of the vector and return the index of the first of them
#define ROSALIA_VECTOR_ADDLEN_IDX(pp_vec, len) (ROSALIA__VECTOR_INTERNAL_VECTOR_MGROW((pp_vec), (len)), (len) > 0 ? (ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length += (n), ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - (n)) : ROSALIA_VECTOR_LEN(pp_vec))

// addlen_ptr(T** pp_vec, size_t len)
// add len uninitialized elements to the end of the vector and return a ptr to the first of them
#define ROSALIA_VECTOR_ADDLEN_PTR(pp_vec, len) (ROSALIA__VECTOR_INTERNAL_VECTOR_MGROW((pp_vec), (len)), (len) > 0 ? (ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length += (len), &(*(pp_vec))[ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - (len)]) : *(pp_vec))

// push(T** pp_vec, T elem)
// add elem to the end of the vector
#define ROSALIA_VECTOR_PUSH(pp_vec, elem) (ROSALIA__VECTOR_INTERNAL_VECTOR_MGROW((pp_vec), 1), (*(pp_vec))[ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length++] = (elem))

// push_n(T** pp_vec, size_t len)
// add len uninitialized elements to the end of the vector
#define ROSALIA_VECTOR_PUSH_N(pp_vec, len) (ROSALIA__VECTOR_INTERNAL_VECTOR_MGROW((pp_vec), (len)), ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length += len)

// T pop(T** pp_vec)
// get the last element of the vector, and remove it at the same time
#define ROSALIA_VECTOR_POP(pp_vec) (ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length--, (*(pp_vec))[ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length])

// pop_n(T** pp_vec, size_t len)
// remove the len last elements from the vector
#define ROSALIA_VECTOR_POP_N(pp_vec, len) (ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length >= len ? ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length -= len : ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length = 0)

// T peek(T** pp_vec, size_t ridx)
// get the element at ridx from the end of the vector, i.e. vec[VEC_LEN(&vec) - 1 - ridx]
#define ROSALIA_VECTOR_PEEK(pp_vec, ridx) ((*(pp_vec))[ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - 1 - (rdix)])

// insert(T** pp_vec, size_t idx, T elem)
// insert elem T such that it is reachable at idx
#define ROSALIA_VECTOR_INSERT(pp_vec, idx, elem) ((ROSALIA_VECTOR_INSERT_N((pp_vec), (idx), 1), (*(pp_vec))[idx] = (elem)))

// insert_n(T** pp_vec, size_t idx, size_t len)
// inserts len uninitialized elements at idx forward
#define ROSALIA_VECTOR_INSERT_N(pp_vec, idx, len) (ROSALIA_VECTOR_ADDLEN((pp_vec), (len)), memmove(&(*(pp_vec))[(idx) + (len)], &(*(pp_vec))[idx], sizeof(**(pp_vec)) * (ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - (len) - (idx))))

// remove(T** pp_vec, size_t idx)
// remove element at idx and copy the rest so the order stays stable
#define ROSALIA_VECTOR_REMOVE(pp_vec, idx) (ROSALIA_VECTOR_REMOVE_N((pp_vec), (idx), 1))

// remove_n(T** pp_vec, size_t idx, size_t len)
// remove len elements at idx forward and copy the rest so the order stays stable
#define ROSALIA_VECTOR_REMOVE_N(pp_vec, idx, len) (memmove(&(*(pp_vec))[idx], &(*(pp_vec))[(idx) + (len)], sizeof(**(pp_vec)) * (ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - (len) - (idx))), ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length -= (len))

// remove_swap(T** pp_vec, size_t idx)
// remove element at idx and swap in an element from the back, order is not stable, but is much faster than copying
#define ROSALIA_VECTOR_REMOVE_SWAP(pp_vec, idx) (ROSALIA_VECTOR_REMOVE_SWAP_N((pp_vec), (idx), 1))

// remove_swap_n(T** pp_vec, size_t idx, size_t len)
// remove len elements at idx forward and swap in element from the back (if required), order is not stable, but is much faster than copying
#define ROSALIA_VECTOR_REMOVE_SWAP_N(pp_vec, idx, len) (memmove(&(*(pp_vec))[idx], &(*(pp_vec))[ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - ((idx) + (len) > ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - (len) ? ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - (idx) - (len) : (len))], sizeof(**(pp_vec)) * ((idx) + (len) > ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - (len) ? ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - (idx) - (len) : (len))), ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length -= (len))

// T last(T** pp_vec)
// get the last element in the vector
#define ROSALIA_VECTOR_LAST(pp_vec) ((*(pp_vec))[ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec)->length - 1])

// clone(T** pp_vec_destination, T** pp_vec_source)
// shallow clones source vector into destination, if source is NULL or empty, then so will the destination be afterwards
#define ROSALIA_VECTOR_CLONE(pp_vec_d, pp_vec_s) (ROSALIA_VECTOR_CREATE((pp_vec_d), ROSALIA_VECTOR_LEN(pp_vec_s)), *(pp_vec_s) != NULL ? (ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec_d)->length = ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec_s)->length, memcpy(*(pp_vec_d), *(pp_vec_s), sizeof(**(pp_vec_s)) * ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(pp_vec_s)->length), 0) : (*(pp_vec_d) = NULL, 0))

#ifdef __cplusplus
}
#endif

#endif

//TODO serialization.h compatibility, layout must be given each time on serialization
/*
usage should look like:
typedef struct thing_s {
    uint32_t* vec1;
} thing;

serialization_layout sl_thing[] = {
    {SL_TYPE_32 | SL_TYPE_VECTOR, offsetof(thing, vec1)},
    {SL_TYPE_STOP},
};
*/

#if defined(ROSALIA_VECTOR_IMPLEMENTATION) && !defined(ROSALIA_VECTOR_H_IMPL)
#define ROSALIA_VECTOR_H_IMPL

#define ROSALIA__VECTOR_INTERNAL(ident) rosalia__vector_internal_##ident

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

//note: p_vec can be NULL in both but VEC_LEN and VEC_CAP can both be used on NULL vectors and return 0

ROSALIA__VECTOR_DEF void* rosalia__vector_internal_grow(void* p_vec, size_t elem_size, size_t add_len, size_t min_cap)
{

    size_t min_len = ROSALIA_VECTOR_LEN(&p_vec) + add_len;

    // capacity has to fit at least all elements
    if (min_len > min_cap) {
        min_cap = min_len;
    }

    // if required capacity is satisfied, pass
    if (min_cap <= ROSALIA_VECTOR_CAP(&p_vec)) {
        return p_vec;
    }

    // doubling capacity, at least 4
    if (min_cap < 2 * ROSALIA_VECTOR_CAP(&p_vec)) {
        min_cap = 2 * ROSALIA_VECTOR_CAP(&p_vec);
    } else if (min_cap < 4) {
        min_cap = 4;
    }

    size_t new_len = ROSALIA_VECTOR_LEN(&p_vec);

    rosalia__internal_vector_info* new_vec = (rosalia__internal_vector_info*)realloc(p_vec == NULL ? NULL : ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(&p_vec), sizeof(rosalia__internal_vector_info) + elem_size * min_cap);

    *new_vec = (rosalia__internal_vector_info){
        .length = new_len,
        .capacity = min_cap,
    };

    return new_vec + 1;
}

// FITLEN: L < C : realloc to fit
// FITLEN: L > C : realloc to fit
// FITCAP: L < C : "expected natural shrink", fit=max(len*2,4)
// FITCAP: L = C : realloc to fit=len
// FITCAP: L > C : truncate length, realloc to fit
ROSALIA__VECTOR_DEF void* rosalia__vector_internal_shrink_to_fit(void* p_vec, size_t elem_size, size_t fit)
{
    if (p_vec == NULL) {
        return NULL;
    }

    size_t len = ROSALIA_VECTOR_LEN(&p_vec);

    if (len > fit) {
        len = fit;
    }
    if (fit < 4) {
        fit = 4;
    }

    rosalia__internal_vector_info* new_vec = (rosalia__internal_vector_info*)realloc(p_vec == NULL ? NULL : ROSALIA__VECTOR_INTERNAL_VECTOR_HEADER(&p_vec), sizeof(rosalia__internal_vector_info) + elem_size * fit);

    *new_vec = (rosalia__internal_vector_info){
        .length = len,
        .capacity = fit,
    };

    return new_vec + 1;
}

#ifdef __cplusplus
}
#endif

#endif
