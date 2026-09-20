#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(LIARA_PLATFORM_SHARED) && !defined(LIARA_PLATFORM_STATIC) && defined(LIARA_PLATFORM_BUILD)
    #error "Building liara-platform requires either LIARA_PLATFORM_SHARED or LIARA_PLATFORM_STATIC"
#endif

#if defined(LIARA_PLATFORM_SHARED) && defined(LIARA_PLATFORM_STATIC)
    #error "You cannot define both LIARA_PLATFORM_SHARED and LIARA_PLATFORM_STATIC"
#endif

#ifdef _WIN32
    #if defined(LIARA_PLATFORM_SHARED)
        #if defined(LIARA_PLATFORM_BUILD)
            #define LIARA_PLATFORM_API __declspec(dllexport)
        #else
            #define LIARA_PLATFORM_API __declspec(dllimport)
        #endif
    #else
        #define LIARA_PLATFORM_API
    #endif
#else
    #ifdef LIARA_PLATFORM_SHARED
        #define LIARA_PLATFORM_API __attribute__((visibility("default")))
    #else
        #define LIARA_PLATFORM_API
    #endif
#endif

#ifdef __cplusplus
}
#endif
