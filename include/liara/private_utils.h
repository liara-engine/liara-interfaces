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

#if defined(__cplusplus) && __cplusplus >= 201402L
    #define LIARA_API_DEPRECATED(msg) [[deprecated(msg)]]
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #define LIARA_API_DEPRECATED(msg) [[deprecated(msg)]]
#elif defined(_MSC_VER)
    #define LIARA_API_DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__clang__) || defined(__GNUC__)
    #define LIARA_API_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
    #define LIARA_API_DEPRECATED(msg)
#endif

#ifdef _WIN32
    #if defined(LIARA_SHARED)
        #if defined(LIARA_BUILD)
            #define LIARA_API __declspec(dllexport)
        #else
            #define LIARA_API __declspec(dllimport)
        #endif
    #else
        #define LIARA_API
    #endif
#else
    #ifdef LIARA_SHARED
        #define LIARA_API __attribute__((visibility("default")))
    #else
        #define LIARA_API
    #endif
#endif

#ifdef __cplusplus
    #define LIARA_STATIC_CAST(type, value) static_cast<type>((value))
#else
    #define LIARA_STATIC_CAST(type, value) ((type)(value))
#endif

#ifdef __cplusplus
    #define LIARA_NULL nullptr
#else
    #define LIARA_NULL ((void*)0)
#endif

#ifdef __cplusplus
    #define LIARA_TYPEDEF(type, name)                 using name = type
    #define LIARA_TYPEDEF_DEPRECATED(type, name, msg) using name LIARA_API_DEPRECATED(msg) = type
    #define LIARA_TYPEDEF_STRUCT(name, ...) \
        using name = struct name {          \
            __VA_ARGS__                     \
        }
#else
    #define LIARA_TYPEDEF(type, name)                 typedef type name
    #define LIARA_TYPEDEF_DEPRECATED(type, name, msg) typedef type name LIARA_API_DEPRECATED(msg)
    #define LIARA_TYPEDEF_STRUCT(name, ...) \
        typedef struct name {               \
            __VA_ARGS__                     \
        } name
#endif

#ifdef __cplusplus
}
#endif
