#pragma once

#include <liara/internal/portability.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * @brief Convert a liara_result_t code to a human-readable string.
 *
 * This function takes a liara_result_t code and returns a string representation of the code. If the code is not
 * recognized, it returns "Unknown liara_result_t code".
 *
 * @param result[in] The liara_result_t code to convert.
 * @return A string representation of the liara_result_t code.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline const char* liara_result_to_string(const liara_result_t result) {
    switch (result) {
#define LIARA_X(name, value) \
    case name: return #name;
        LIARA_RESULT_LIST(LIARA_X)
#undef LIARA_X
        default: return "Unknown liara_result_t code";
    }
}

#ifdef __cplusplus
}
#endif
