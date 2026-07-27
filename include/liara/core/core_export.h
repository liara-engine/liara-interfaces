#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
    #if defined(LIARA_CORE_SHARED)
        #if defined(LIARA_CORE_BUILD)
            #define LIARA_CORE_API __declspec(dllexport)
        #else
            #define LIARA_CORE_API __declspec(dllimport)
        #endif
    #else
        #define LIARA_CORE_API
    #endif
#else
    #ifdef LIARA_CORE_SHARED
        #define LIARA_CORE_API __attribute__((visibility("default")))
    #else
        #define LIARA_CORE_API
    #endif
#endif

#ifdef __cplusplus
}
#endif
