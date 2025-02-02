#include <stdlib.h>

#include "rosalia/alloc.h"
#include "rosalia/heapstackify.h"

#ifdef __cplusplus
extern "C" {
#endif

void heapstackify_call_mgr(allocator* allocp, hs_data* hs)
{
    hs_data* callstack = hs;
    while (callstack) {
        hs_data* ret_sf = callstack->func(allocp, callstack);
        if (ret_sf != NULL) {
            callstack = ret_sf;
            continue;
        }
        hs_data* del_sf = callstack;
        callstack = callstack->prev;
        allocp->free(allocp, del_sf);
    }
}

#ifdef __cplusplus
}
#endif
