#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
    #define LIARA_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#elif __STDC_VERSION__ >= 202311L
    #define LIARA_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#else
    #define LIARA_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#endif

#ifdef __cplusplus
}
#endif
