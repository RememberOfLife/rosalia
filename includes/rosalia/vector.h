#ifndef ROSALIA_VECTOR_H_INCLUDE
#define ROSALIA_VECTOR_H_INCLUDE

#include <stddef.h>
#include <stdint.h>

#ifdef ROSALIA_VECTOR_STATIC
#define ROSALIA__DEC static
#define ROSALIA__DEC_EXT static
#define ROSALIA__DEF static
#else
#define ROSALIA__DEC
#define ROSALIA__DEC_EXT extern
#define ROSALIA__DEF
#endif

#ifdef ROSALIA_VECTOR_DECORATE
#define ROSALIA__DECORATE(ident) ROSALIA_VECTOR_DECORATE(ident)
#else
#define ROSALIA__DECORATE(ident) ident
#endif

#define ROSALIA_VECTOR_VERSION_MAJOR 0
#define ROSALIA_VECTOR_VERSION_MINOR 2
#define ROSALIA_VECTOR_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

// typed vector macro api taken from: https://github.com/nothings/stb/blob/master/stb_ds.h in the public domain
//TODO api is slightly adapted, make sure to read each the macros usage instructions after the short names

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
#define VEC_ADDLEN ROSALIA_VECTOR_ADDLEN //TODO need this, its really just one of the 2 below and discarding the return
#define VEC_ADDLEN_IDX ROSALIA_VECTOR_ADDLEN_IDX
#define VEC_ADDLEN_PTR ROSALIA_VECTOR_ADDLEN_PTR
#define VEC_PUSH ROSALIA_VECTOR_PUSH
//TODO pushn is just addlen
#define VEC_POP ROSALIA_VECTOR_POP
//TODO want popn?
//TODO peek with idx from back?
#define VEC_INSERT ROSALIA_VECTOR_INSERT
#define VEC_INSERT_N ROSALIA_VECTOR_INSERT_N
#define VEC_REMOVE ROSALIA_VECTOR_REMOVE
#define VEC_REMOVE_N ROSALIA_VECTOR_REMOVE_N
#define VEC_REMOVE_SWAP ROSALIA_VECTOR_REMOVE_SWAP
#define VEC_REMOVE_SWAP_N ROSALIA_VECTOR_REMOVE_SWAP_N
#define VEC_LAST ROSALIA_VECTOR_LAST
#endif

//TODO alloc.h compat

//TODO check all of these for correct behaviour

typedef struct rosalia__internal_vector_info_s {
    size_t length;
    size_t capacity;
} rosalia__internal_vector_info;

#define ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec) (((rosalia__internal_vector_info*)*(pp_vec)) - 1)

// takes a vector and returns a vector that:
// has at least enough space to fit additional add_len elements and has a minimum capacity of min_cap
ROSALIA__DEC void* rosalia__internal_vector_grow(void* p_vec, size_t elem_size, size_t add_len, size_t min_cap);

// takes a vector and returns a vector that:
// has enough space for fit many elements, unused capacity is released
ROSALIA__DEC void* rosalia__internal_vector_shrink_to_fit(void* p_vec, size_t elem_size, size_t fit);

#define ROSALIA__INTERNAL_VECTOR_GROW(pp_vec, len, cap) (*(pp_vec) = rosalia__internal_vector_grow(*(pp_vec), sizeof(**(pp_vec)), (len), (cap)))

#define ROSALIA__INTERNAL_VECTOR_MGROW(pp_vec, len) ((*(pp_vec) == NULL || ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length + (len) > ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->capacity) ? (ROSALIA__INTERNAL_VECTOR_GROW((pp_vec), (len), 0), 0) : 0)

// create
//TODO description
#define ROSALIA_VECTOR_CREATE(pp_vec, cap) (ROSALIA__INTERNAL_VECTOR_GROW((pp_vec), 0, (cap)))

// destroy
//TODO description
#define ROSALIA_VECTOR_DESTROY(pp_vec) ((void)(*(pp_vec) != NULL ? free(ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)) : (void)0), *(pp_vec) = NULL)

// cap
//TODO description
#define ROSALIA_VECTOR_CAP(pp_vec) (*(pp_vec) != NULL ? ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->capacity : 0)

// setcap
//TODO description
#define ROSALIA_VECTOR_SETCAP(pp_vec, cap) (ROSALIA__INTERNAL_VECTOR_GROW((pp_vec), 0, (cap)))

// fitcap
//TODO description
#define ROSALIA_VECTOR_FITCAP(pp_vec) (*(pp_vec) = rosalia__internal_vector_shrink_to_fit(*(pp_vec), sizeof(**(pp_vec)), ROSALIA_VECTOR_LEN(pp_vec) < ROSALIA_VECTOR_CAP(pp_vec) ? 2 * ROSALIA_VECTOR_LEN(pp_vec) : ROSALIA_VECTOR_CAP(pp_vec)))

// len
//TODO description
#define ROSALIA_VECTOR_LEN(pp_vec) (*(pp_vec) != NULL ? ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length : 0)

// setlen
//TODO description
#define ROSALIA_VECTOR_SETLEN(pp_vec, len) ((ROSALIA_VECTOR_CAP(pp_vec) < (size_t)(len) ? ROSALIA_VECTOR_SETCAP((pp_vec), (size_t)(len)), 0 : 0), *(pp_vec) != NULL ? ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length = (size_t)(len) : 0)

// fitlen
//TODO description
#define ROSALIA_VECTOR_FITLEN(pp_vec) (*(pp_vec) = rosalia__internal_vector_shrink_to_fit(*(pp_vec), sizeof(**(pp_vec)), ROSALIA_VECTOR_LEN(pp_vec)))

// addlen
//TODO description
#define ROSALIA_VECTOR_ADDLEN(pp_vec, len) (ROSALIA__INTERNAL_VECTOR_MGROW((pp_vec), (len)), ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length += len)

// addlen_idx
//TODO description
#define ROSALIA_VECTOR_ADDLEN_IDX(pp_vec, len) (ROSALIA__INTERNAL_VECTOR_MGROW((pp_vec), (len)), (len) > 0 ? (ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length += (n), ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - (n)) : ROSALIA_VECTOR_LEN(pp_vec))

// addlen_ptr
//TODO description
#define ROSALIA_VECTOR_ADDLEN_PTR(pp_vec, len) (ROSALIA__INTERNAL_VECTOR_MGROW((pp_vec), (len)), (len) > 0 ? (ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length += (len), &(*(pp_vec))[ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - (len)]) : *(pp_vec))

// push
//TODO description
#define ROSALIA_VECTOR_PUSH(pp_vec, elem) (ROSALIA__INTERNAL_VECTOR_MGROW((pp_vec), 1), (*(pp_vec))[ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length++] = (elem))

// pop
//TODO description
#define ROSALIA_VECTOR_POP(pp_vec) (ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length--, (*(pp_vec))[ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length])

// insert
//TODO description
#define ROSALIA_VECTOR_INSERT(pp_vec, idx, elem) ((ROSALIA_VECTOR_INSERT_N((pp_vec), (idx), 1), (*(pp_vec))[idx] = (elem)))

// insert_n
//TODO description
#define ROSALIA_VECTOR_INSERT_N(pp_vec, idx, len) (ROSALIA_VECTOR_ADDLEN((pp_vec), (len)), memmove(&(*(pp_vec))[(idx) + (len)], &(*(pp_vec))[idx], sizeof(**(pp_vec)) * (ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - (len) - (idx))))

// remove
//TODO description
#define ROSALIA_VECTOR_REMOVE(pp_vec, idx) (ROSALIA_VECTOR_REMOVE((pp_vec), (idx), 1))

// remove_n
//TODO description
#define ROSALIA_VECTOR_REMOVE_N(pp_vec, idx, len) (memmove(&(*(pp_vec))[idx], &(*(pp_vec))[(idx) + (len)], sizeof(**(pp_vec)) * (ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - (len) - (idx))), ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length -= (len))

// remove_swap
//TODO description
#define ROSALIA_VECTOR_REMOVE_SWAP(pp_vec, idx) (ROSALIA_VECTOR_REMOVE_SWAP_N((pp_vec), (idx), 1))

// remove_swap_n
//TODO description
#define ROSALIA_VECTOR_REMOVE_SWAP_N(pp_vec, idx, len) (memmove(&(*(pp_vec))[idx], &(*(pp_vec))[ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - ((idx) + (len) > ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - (len) ? ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - (idx) - (len) : (len))], (idx) + (len) > ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - (len) ? ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - (idx) - (len) : (len)), ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length -= (len))

// last
//TODO description
#define ROSALIA_VECTOR_LAST(pp_vec) ((*(pp_vec))[ROSALIA__INTERNAL_VECTOR_HEADER(pp_vec)->length - 1])

#ifdef __cplusplus
}
#endif

#endif

//TODO serialization.h compatibility, layout must be given each time on serialization

#ifdef ROSALIA_VECTOR_IMPLEMENTATION

#define ROSALIA__INTERNAL(ident) rosalia__internal_##ident

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

ROSALIA__DEF void* rosalia__internal_vector_grow(void* p_vec, size_t elem_size, size_t add_len, size_t min_cap)
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

    rosalia__internal_vector_info* new_vec = realloc(p_vec == NULL ? NULL : ROSALIA__INTERNAL_VECTOR_HEADER(&p_vec), sizeof(rosalia__internal_vector_info) + elem_size * min_cap);

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
ROSALIA__DEF void* rosalia__internal_vector_shrink_to_fit(void* p_vec, size_t elem_size, size_t fit)
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

    rosalia__internal_vector_info* new_vec = realloc(p_vec == NULL ? NULL : ROSALIA__INTERNAL_VECTOR_HEADER(&p_vec), sizeof(rosalia__internal_vector_info) + elem_size * fit);

    *new_vec = (rosalia__internal_vector_info){
        .length = len,
        .capacity = fit,
    };

    return new_vec + 1;
}

#ifdef __cplusplus
}
#endif

#undef ROSALIA__INTERNAL
#endif

#undef ROSALIA__DEC
#undef ROSALIA__DEC_EXT
#undef ROSALIA__DEF
#undef ROSALIA__DECORATE
