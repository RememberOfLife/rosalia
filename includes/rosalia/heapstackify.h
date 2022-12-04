#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "rosalia/alloc.h"

#ifdef __cplusplus
extern "C" {
#endif

// for each macro
#define FE_0(WHAT)
#define FE_1(WHAT, X) WHAT(X)
#define FE_2(WHAT, X, ...) WHAT(X) FE_1(WHAT, __VA_ARGS__)
#define FE_3(WHAT, X, ...) WHAT(X) FE_2(WHAT, __VA_ARGS__)
#define FE_4(WHAT, X, ...) WHAT(X) FE_3(WHAT, __VA_ARGS__)
#define FE_5(WHAT, X, ...) WHAT(X) FE_4(WHAT, __VA_ARGS__)
#define FE_6(WHAT, X, ...) WHAT(X) FE_5(WHAT, __VA_ARGS__)
#define FE_7(WHAT, X, ...) WHAT(X) FE_6(WHAT, __VA_ARGS__)
#define FE_8(WHAT, X, ...) WHAT(X) FE_7(WHAT, __VA_ARGS__)
#define FE_9(WHAT, X, ...) WHAT(X) FE_8(WHAT, __VA_ARGS__)
#define FE_10(WHAT, X, ...) WHAT(X) FE_9(WHAT, __VA_ARGS__)
#define FE_11(WHAT, X, ...) WHAT(X) FE_10(WHAT, __VA_ARGS__)
#define FE_12(WHAT, X, ...) WHAT(X) FE_11(WHAT, __VA_ARGS__)
#define FE_13(WHAT, X, ...) WHAT(X) FE_12(WHAT, __VA_ARGS__)
#define FE_14(WHAT, X, ...) WHAT(X) FE_13(WHAT, __VA_ARGS__)
#define FE_15(WHAT, X, ...) WHAT(X) FE_14(WHAT, __VA_ARGS__)
#define FE_16(WHAT, X, ...) WHAT(X) FE_15(WHAT, __VA_ARGS__)
#define GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, NAME, ...) NAME
#define FOR_EACH(action, ...)                                                                                                               \
    GET_MACRO(_0, __VA_ARGS__, FE_16, FE_15, FE_14, FE_13, FE_12, FE_11, FE_10, FE_9, FE_8, FE_7, FE_6, FE_5, FE_4, FE_3, FE_2, FE_1, FE_0) \
    (action, __VA_ARGS__)

typedef struct hs_data_s hs_data;

typedef hs_data* hs_func_t(allocator* _hs_allocp_, hs_data* _hs_data_);

struct hs_data_s {
    hs_data* prev;
    hs_func_t* func;
    void* retvloc; // func writes its return value to this address
    uint32_t state;
};

#define HS_BEGIN case 0:

//TODO replace retvloc by offset of stackframe, CALL_EMPLACE asserts that this offset is guaranteed not to be out of bounds, also HS_RETURN does pointer addition, also need initial HS_FUNC

#define HS_CALL_EMPLACE(rloc, name, param_list...)                                                                         \
    do {                                                                                                                   \
        _hs_frame_##name##_* new_sf = (_hs_frame_##name##_*)_hs_allocp_->malloc(_hs_allocp_, sizeof(_hs_frame_##name##_)); \
        *new_sf = (_hs_frame_##name##_){                                                                                   \
            ._hs_data_ = (hs_data){                                                                                        \
                .state = 0,                                                                                                \
                .prev = _hs_data_,                                                                                         \
                .func = name,                                                                                              \
                .retvloc = (rloc),                                                                                         \
            },                                                                                                             \
            .params = (_hs_params_##name##_){param_list},                                                                  \
        };                                                                                                                 \
        _hs_data_->state = __LINE__;                                                                                       \
        return &new_sf->_hs_data_;                                                                                         \
        case __LINE__:;                                                                                                    \
    } while (0)

#define HS_RETURN(rv)                                   \
    do {                                                \
        *((_hs_local_retv_*)_hs_data_->retvloc) = (rv); \
        return NULL;                                    \
    } while (0)

#define HS_END     \
    default:       \
        assert(0); \
        }          \
        return NULL

#define HS_FUNC_EXPAND_PARAMS_SEMICOLONIFY(v) v;
#define HS_FUNC_EXPAND_PARAMS(param_list...) \
    FOR_EACH(HS_FUNC_EXPAND_PARAMS_SEMICOLONIFY, param_list)

#define HS_FUNC_DECL(name, context_struct, return_type, param_list...) \
    typedef context_struct _hs_context_##name##_;                      \
    typedef struct _hs_params_##name##_s_ {                            \
        HS_FUNC_EXPAND_PARAMS(param_list)                              \
    } _hs_params_##name##_;                                            \
    typedef struct _hs_frame_##name##_s_ {                             \
        hs_data _hs_data_;                                             \
        _hs_context_##name##_ context;                                 \
        _hs_params_##name##_ params;                                   \
    } _hs_frame_##name##_;                                             \
    typedef return_type _hs_retv_##name##_;                            \
    hs_data* name(allocator* _hs_allocp_, hs_data* _hs_data_);

#define HS_FUNC_DEF(name, context_struct, return_type, param_list...)           \
    typedef context_struct _hs_context_##name##_;                               \
    typedef struct _hs_params_##name##_s_ {                                     \
        HS_FUNC_EXPAND_PARAMS(param_list)                                       \
    } _hs_params_##name##_;                                                     \
    typedef struct _hs_frame_##name##_s_ {                                      \
        hs_data _hs_data_;                                                      \
        _hs_context_##name##_ context;                                          \
        _hs_params_##name##_ params;                                            \
    } _hs_frame_##name##_;                                                      \
    typedef return_type _hs_retv_##name##_;                                     \
    hs_data* name(allocator* _hs_allocp_, hs_data* _hs_data_)                   \
    {                                                                           \
        typedef _hs_retv_##name##_ _hs_local_retv_;                             \
        _hs_context_##name##_* c = &((_hs_frame_##name##_*)_hs_data_)->context; \
        _hs_params_##name##_* p = &((_hs_frame_##name##_*)_hs_data_)->params;   \
        switch (_hs_data_->state)

void heapstackify_call_mgr(allocator* allocp, hs_data* hs);

#define HS_ENTRY_CALL_EMPLACE(allocp, rloc, name, param_list...)                                                     \
    do {                                                                                                             \
        _hs_frame_##name##_* new_sf = (_hs_frame_##name##_*)(allocp)->malloc((allocp), sizeof(_hs_frame_##name##_)); \
        *new_sf = (_hs_frame_##name##_){                                                                             \
            ._hs_data_ = (hs_data){                                                                                  \
                .state = 0,                                                                                          \
                .prev = NULL,                                                                                        \
                .func = name,                                                                                        \
                .retvloc = (rloc),                                                                                   \
            },                                                                                                       \
            .params = (_hs_params_##name##_){param_list},                                                            \
        };                                                                                                           \
        heapstackify_call_mgr((allocp), &new_sf->_hs_data_);                                                         \
    } while (0)

#ifdef __cplusplus
}
#endif
