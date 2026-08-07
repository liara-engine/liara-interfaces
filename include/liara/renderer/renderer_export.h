#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(LIARA_RENDERER_SHARED) && !defined(LIARA_RENDERER_STATIC)
    #error "You must define either LIARA_RENDERER_SHARED or LIARA_RENDERER_STATIC"
#endif

#if defined(LIARA_RENDERER_SHARED) && defined(LIARA_RENDERER_STATIC)
    #error "You cannot define both LIARA_RENDERER_SHARED and LIARA_RENDERER_STATIC"
#endif

#ifdef _WIN32
    #if defined(LIARA_RENDERER_SHARED)
        #if defined(LIARA_RENDERER_BUILD)
            #define LIARA_RENDERER_API __declspec(dllexport)
        #else
            #define LIARA_RENDERER_API __declspec(dllimport)
        #endif
    #else
        #define LIARA_RENDERER_API
    #endif
#else
    #ifdef LIARA_RENDERER_SHARED
        #define LIARA_RENDERER_API __attribute__((visibility("default")))
    #else
        #define LIARA_RENDERER_API
    #endif
#endif

#ifdef __cplusplus
}
#endif
