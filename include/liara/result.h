#pragma once

#include <liara/private_utils.h>

#include LIARA_INCLUDE_STD(stdint)

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Remove (marked as deprecated in v0.1.1)
/**
 * @deprecated: This type is deprecated and will be removed in future versions. Use `liara_result_t` instead. \
 */
LIARA_TYPEDEF_DEPRECATED(
    int32_t,
    liara_result,
    "This type is deprecated and will be removed in future versions. Use `liara_result_t` instead.");
LIARA_TYPEDEF(int32_t, liara_result_t);

#define LIARA_RESULT_LIST(X)                                                      \
    /* 0 : Success */                                                             \
    X(LIARA_RESULT_SUCCESS, 0)                                                    \
    /* -1 : Basic failure, please prefer more specific error codes if possible */ \
    X(LIARA_RESULT_FAILED, -1)                                                    \
    /* -2 to -99 : General errors */                                              \
    X(LIARA_RESULT_NOT_IMPLEMENTED, -2)                                           \
    X(LIARA_RESULT_NOT_SUPPORTED, -3)                                             \
    X(LIARA_RESULT_INVALID_STATE, -4)                                             \
    X(LIARA_RESULT_VERSION_MISMATCH, -5)                                          \
    X(LIARA_RESULT_TIMEOUT, -6)                                                   \
    X(LIARA_RESULT_CANCELED, -7)                                                  \
    X(LIARA_RESULT_PERMISSION_DENIED, -8)                                         \
    /* -100 to -199 : Argument errors */                                          \
    X(LIARA_RESULT_INVALID_ARGUMENT, -100)                                        \
    X(LIARA_RESULT_OUT_OF_RANGE, -101)                                            \
    X(LIARA_RESULT_NULL_POINTER, -102)                                            \
    X(LIARA_RESULT_UNEXPECTED_VALUE, -103)                                        \
    X(LIARA_RESULT_INVALID_HANDLE, -104)                                          \
    /* -200 to -299 : Resource errors */                                          \
    X(LIARA_RESULT_OUT_OF_MEMORY, -200)                                           \
    X(LIARA_RESULT_RESOURCE_BUSY, -201)                                           \
    X(LIARA_RESULT_RESOURCE_NOT_FOUND, -202)                                      \
    X(LIARA_RESULT_RESOURCE_ALREADY_EXISTS, -203)                                 \
    X(LIARA_RESULT_RESOURCE_CORRUPTED, -204)                                      \
    /* -300 to -399 : File and I/O errors */                                      \
    X(LIARA_RESULT_IO_ERROR, -300)                                                \
    X(LIARA_RESULT_FILE_NOT_FOUND, -301)                                          \
    X(LIARA_RESULT_FILE_ALREADY_EXISTS, -302)                                     \
    X(LIARA_RESULT_FILE_ACCESS_DENIED, -303)

/* Reserved for future use */

enum liara_result_codes {

#define LIARA_X(name, value) name = value,
    LIARA_RESULT_LIST(LIARA_X)
#undef LIARA_X
};

// TODO: Update this function to use `liara_result_t` instead of `liara_result` in v0.2.0
/**
 * @deprecated This function is deprecated and its signature will use `liara_result_t` instead of `liara_result` in
 * v0.2.0. Please update your code accordingly.
 * @brief Convert a liara_result code to a human-readable string.
 *
 * This function takes a liara_result code and returns a string representation of the code. If the code is not
 * recognized, it returns "Unknown liara_result code".
 *
 * @param result[in] The liara_result code to convert.
 * @return A string representation of the liara_result code.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED("This function is deprecated and its signature will use liara_result_t instead of liara_result in "
                     "v0.2.0. Please update your code accordingly.")

static inline const char* liara_result_to_string(const liara_result result) {
    switch (result) {
#define LIARA_X(name, value) \
    case name: return #name;
        LIARA_RESULT_LIST(LIARA_X)
#undef LIARA_X
        default: return "Unknown liara_result code";
    }
}

#ifdef __cplusplus
}
#endif
