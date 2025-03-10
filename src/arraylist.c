#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rosalia/arraylist.h"

#ifdef __cplusplus
extern "C" {
#endif

//note: p_vec can be NULL in both but VEC_LEN and VEC_CAP can both be used on NULL vectors and return 0
// returns NULl if p_vec is NULL
void* rosalia__arraylist_grow_impl(void* p_vec, size_t elem_size, size_t add_len, size_t min_cap)
{
    size_t min_len = ROSA_ARRAYLIST_LEN(&p_vec) + add_len;

    // capacity has to fit at least all elements
    if (min_len > min_cap) {
        min_cap = min_len;
    }

    // if required capacity is satisfied, pass
    if (min_cap <= ROSA_ARRAYLIST_CAP(&p_vec)) {
        return p_vec;
    }

    // doubling capacity, at least 4
    if (min_cap < 2 * ROSA_ARRAYLIST_CAP(&p_vec)) {
        min_cap = 2 * ROSA_ARRAYLIST_CAP(&p_vec);
    } else if (min_cap < 4) {
        min_cap = 4;
    }

    size_t new_len = ROSA_ARRAYLIST_LEN(&p_vec);

    rosa__arraylist_info* new_vec = (rosa__arraylist_info*)realloc(p_vec == NULL ? NULL : ROSA__ARRAYLIST_GET_INFO(&p_vec), sizeof(rosa__arraylist_info) + elem_size * min_cap);

    *new_vec = (rosa__arraylist_info){
        .length = new_len,
        .capacity = min_cap,
    };

    return new_vec + 1;
}

// returns NULl if p_vec is NULL
// FITLEN: L < C : realloc to fit
// FITLEN: L > C : realloc to fit
// FITCAP: L < C : "expected natural shrink", fit=max(len*2,4)
// FITCAP: L = C : realloc to fit=len
// FITCAP: L > C : truncate length, realloc to fit
void* rosalia__arraylist_shrink_to_fit(void* p_vec, size_t elem_size, size_t fit)
{
    if (p_vec == NULL) {
        return NULL;
    }

    size_t len = ROSA_ARRAYLIST_LEN(&p_vec);

    if (len > fit) {
        len = fit;
    }
    if (fit < 4) {
        fit = 4;
    }

    rosa__arraylist_info* new_vec = (rosa__arraylist_info*)realloc(p_vec == NULL ? NULL : ROSA__ARRAYLIST_GET_INFO(&p_vec), sizeof(rosa__arraylist_info) + elem_size * fit);

    *new_vec = (rosa__arraylist_info){
        .length = len,
        .capacity = fit,
    };

    return new_vec + 1;
}

#ifdef __cplusplus
}
#endif
