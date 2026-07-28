/**
 * @file version.h
 * @brief Generic 32-bit version encoding/decoding and utilities.
 */

#pragma once

#include <liara/private_utils.h>
#include <liara/result.h>

#ifndef __cplusplus
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
#else
    #include <cstddef>
    #include <cstdint>
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define LIARA_VERSION_MAJOR_BITS 10U
#define LIARA_VERSION_MINOR_BITS 10U
#define LIARA_VERSION_PATCH_BITS 12U

#define LIARA_VERSION_MAJOR_SHIFT (LIARA_VERSION_MINOR_BITS + LIARA_VERSION_PATCH_BITS)
#define LIARA_VERSION_MINOR_SHIFT (LIARA_VERSION_PATCH_BITS)
#define LIARA_VERSION_PATCH_SHIFT (0U)

#define LIARA_VERSION_MAJOR_MASK ((1U << LIARA_VERSION_MAJOR_BITS) - 1U)
#define LIARA_VERSION_MINOR_MASK ((1U << LIARA_VERSION_MINOR_BITS) - 1U)
#define LIARA_VERSION_PATCH_MASK ((1U << LIARA_VERSION_PATCH_BITS) - 1U)

LIARA_STATIC_ASSERT(LIARA_VERSION_MAJOR_BITS + LIARA_VERSION_MINOR_BITS + LIARA_VERSION_PATCH_BITS <= 32U,
                    "Version components exceed 32 bits");

/**
 * @brief Macro to stringify a value.
 *
 * This macro converts the given value into a string literal. It is useful for generating string representations of
 * version numbers or other constants.
 *
 * @param[in] x The value to be converted into a string literal.
 * @return A string literal representing the value of @c x.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#ifndef LIARA_STRINGIFY
    #define LIARA_STRINGIFY(x) #x
#endif

/**
 * @brief Macro to convert a value into a string literal.
 *
 * This macro is a wrapper around the LIARA_STRINGIFY macro. It is provided for convenience and can be used to generate
 * string representations of version numbers or other constants.
 *
 * @param[in] x The value to be converted into a string literal.
 * @return A string literal representing the value of @c x.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#ifndef LIARA_TOSTRING
    #define LIARA_TOSTRING(x) LIARA_STRINGIFY(x)
#endif

/**
 * @brief Create a version number from major, minor, and patch components, with NO safety checks.
 *
 * This macro combines the major, minor, and patch version numbers into a single 32-bit unsigned integer.
 *
 * @warning This macro does not perform any safety checks on the input values. It is the caller's responsibility to
 * ensure that the major, minor, and patch values are within their respective valid ranges:
 * - @c major must be between 0 and 1023 (inclusive)
 * - @c minor must be between 0 and 1023 (inclusive)
 * - @c patch must be between 0 and 4095 (inclusive)
 *
 * @param[in] major The major version number (0-1023).
 * @param[in] minor The minor version number (0-1023).
 * @param[in] patch The patch version number (0-4095).
 * @return A 32-bit unsigned integer representing the combined version.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#define LIARA_MAKE_VERSION_UNSAFE(major, minor, patch)                                      \
    (LIARA_STATIC_CAST(uint32_t,                                                            \
                       ((LIARA_STATIC_CAST(uint32_t, major) << LIARA_VERSION_MAJOR_SHIFT)   \
                        | (LIARA_STATIC_CAST(uint32_t, minor) << LIARA_VERSION_MINOR_SHIFT) \
                        | (LIARA_STATIC_CAST(uint32_t, patch) << LIARA_VERSION_PATCH_SHIFT))))

// TODO: Update this function to use `liara_result_t` instead of `liara_result` in v0.2.0
/**
 * @deprecated This function is deprecated and its signature will use `liara_result_t` instead of `liara_result` in
 * v0.2.0. Please update your code accordingly.
 * @brief Inline function to create a version number from major, minor, and patch components.
 *
 * This function combines the major, minor, and patch version numbers into a single 32-bit unsigned integer.
 * It performs safety checks to ensure that the input values are within their respective valid ranges:
 * - @c major must be between 0 and 1023 (inclusive)
 * - @c minor must be between 0 and 1023 (inclusive)
 * - @c patch must be between 0 and 4095 (inclusive)
 *
 * If the input values are valid, the combined version is stored in the output parameter @c out_version.
 * If any of the input values are out of range or if @c out_version is LIARA_NULL, the function returns an appropriate
 * liara_result error code and does not modify @c out_version.
 *
 * The allocation of the output parameter is the responsibility of the caller, and it must be a valid pointer to a
 * uint32_t.
 *
 * @param[in] major The major version number (0-1023).
 * @param[in] minor The minor version number (0-1023).
 * @param[in] patch The patch version number (0-4095).
 * @param[out] out_version Pointer to a uint32_t where the combined version will be stored if successful.
 * @return LIARA_RESULT_SUCCESS if the version was successfully created and stored in @c out_version,
 *         LIARA_RESULT_NULL_POINTER if @c out_version is LIARA_NULL,
 *         LIARA_RESULT_OUT_OF_RANGE if any of the input values are out of range.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED("This function is deprecated and its signature will use `liara_result_t` instead of "
                     "`liara_result` in v0.2.0. Please update your code accordingly.")

static inline liara_result liara_try_make_version(const uint32_t major,
                                                  const uint32_t minor,
                                                  const uint32_t patch,
                                                  uint32_t* out_version) {
    if (out_version == LIARA_NULL) { return LIARA_RESULT_NULL_POINTER; }
    if (major > LIARA_VERSION_MAJOR_MASK) { return LIARA_RESULT_OUT_OF_RANGE; }
    if (minor > LIARA_VERSION_MINOR_MASK) { return LIARA_RESULT_OUT_OF_RANGE; }
    if (patch > LIARA_VERSION_PATCH_MASK) { return LIARA_RESULT_OUT_OF_RANGE; }
    *out_version = LIARA_MAKE_VERSION_UNSAFE(major, minor, patch);
    return LIARA_RESULT_SUCCESS;
}

/**
 * @brief Extract the major version component from a combined version number.
 *
 * This macro extracts the major version number from a combined version number by shifting and masking.
 *
 * @param[in] v The combined version number from which to extract the major version.
 * @return The major version number (0-1023).
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#define LIARA_VERSION_MAJOR(v) \
    ((LIARA_STATIC_CAST(uint32_t, v) >> LIARA_VERSION_MAJOR_SHIFT) & LIARA_VERSION_MAJOR_MASK)

/**
 * @brief Extract the minor version component from a combined version number.
 *
 * This macro extracts the minor version number from a combined version number by shifting and masking.
 *
 * @param[in] v The combined version number from which to extract the minor version.
 * @return The minor version number (0-1023).
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#define LIARA_VERSION_MINOR(v) \
    ((LIARA_STATIC_CAST(uint32_t, v) >> LIARA_VERSION_MINOR_SHIFT) & LIARA_VERSION_MINOR_MASK)

/**
 * @brief Extract the patch version component from a combined version number.
 *
 * This macro extracts the patch version number from a combined version number by shifting and masking.
 *
 * @param[in] v The combined version number from which to extract the patch version.
 * @return The patch version number (0-4095).
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#define LIARA_VERSION_PATCH(v) \
    ((LIARA_STATIC_CAST(uint32_t, v) >> LIARA_VERSION_PATCH_SHIFT) & LIARA_VERSION_PATCH_MASK)

/**
 * @brief Inline function to extract the major version component from a combined version number.
 *
 * This function is a wrapper around the LIARA_VERSION_MAJOR macro for other languages that may not support macros
 * or for better type safety in C. It extracts the major version number from a combined version number by shifting and
 * masking.
 *
 * @param[in] v The combined version number from which to extract the major version.
 * @return The major version number (0-1023).
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline uint32_t liara_version_major(const uint32_t v) { return LIARA_VERSION_MAJOR(v); }

/**
 * @brief Inline function to extract the minor version component from a combined version number.
 *
 * This function is a wrapper around the LIARA_VERSION_MINOR macro for other languages that may not support macros
 * or for better type safety in C. It extracts the minor version number from a combined version number by shifting and
 * masking.
 *
 * @param[in] v The combined version number from which to extract the minor version.
 * @return The minor version number (0-1023).
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline uint32_t liara_version_minor(const uint32_t v) { return LIARA_VERSION_MINOR(v); }

/**
 * @brief Inline function to extract the patch version component from a combined version number.
 *
 * This function is a wrapper around the LIARA_VERSION_PATCH macro for other languages that may not support macros
 * or for better type safety in C. It extracts the patch version number from a combined version number by shifting and
 * masking.
 *
 * @param[in] v The combined version number from which to extract the patch version.
 * @return The patch version number (0-4095).
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline uint32_t liara_version_patch(const uint32_t v) { return LIARA_VERSION_PATCH(v); }

/**
 * @deprecated This function is deprecated and will be removed in future versions. Use `liara_version_compare` instead.
 * @brief Inline function to compare two version numbers for equality, with an option to ignore the patch version.
 *
 * This function compares two version numbers for equality. If the `ignore_patch` parameter is set to a non-zero value,
 * the comparison will only consider the major and minor version components, ignoring the patch version.
 *
 * @param[in] v1 The first version number to compare.
 * @param[in] v2 The second version number to compare.
 * @param[in] ignore_patch If non-zero, ignore the patch version in the comparison.
 * @return Non-zero if the versions are considered equal based on the specified criteria, zero otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and will be removed in future versions. Use liara_version_compare instead.")

static inline bool liara_version_eq(const uint32_t v1, const uint32_t v2, const bool ignore_patch) {
    if (ignore_patch) {
        return LIARA_VERSION_MAJOR(v1) == LIARA_VERSION_MAJOR(v2) && LIARA_VERSION_MINOR(v1) == LIARA_VERSION_MINOR(v2);
    }
    return v1 == v2;
}

/**
 * @deprecated This function is deprecated and will be removed in future versions. Use `liara_version_compare` instead.
 * @brief Inline function to compare two version numbers for inequality, with an option to ignore the patch version.
 *
 * This function compares two version numbers for inequality. If the `ignore_patch` parameter is set to a non-zero
 * value, the comparison will only consider the major and minor version components, ignoring the patch version.
 *
 * @param[in] v1 The first version number to compare.
 * @param[in] v2 The second version number to compare.
 * @param[in] ignore_patch If non-zero, ignore the patch version in the comparison.
 * @return Non-zero if the versions are considered not equal based on the specified criteria, zero otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and will be removed in future versions. Use liara_version_compare instead.")

static inline bool liara_version_neq(const uint32_t v1, const uint32_t v2, const bool ignore_patch) {
    return !liara_version_eq(v1, v2, ignore_patch) != 0;
}

/**
 * @deprecated This function is deprecated and will be removed in future versions. Use `liara_version_compare` instead.
 * @brief Inline function to compare two version numbers to determine if the first is less than the second, with an
 * option to ignore the patch version.
 *
 * This function compares two version numbers to determine if the first is less than the second. If the `ignore_patch`
 * parameter is set to a non-zero value, the comparison will only consider the major and minor version components,
 * ignoring the patch version.
 *
 * @param[in] v1 The first version number to compare.
 * @param[in] v2 The second version number to compare.
 * @param[in] ignore_patch If non-zero, ignore the patch version in the comparison.
 * @return Non-zero if the first version is considered less than the second based on the specified criteria, zero
 * otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and will be removed in future versions. Use liara_version_compare instead.")

static inline bool liara_version_lt(const uint32_t v1, const uint32_t v2, const bool ignore_patch) {
    if (ignore_patch) {
        if (LIARA_VERSION_MAJOR(v1) < LIARA_VERSION_MAJOR(v2)) { return true; }
        if (LIARA_VERSION_MAJOR(v1) > LIARA_VERSION_MAJOR(v2)) { return false; }
        return LIARA_VERSION_MINOR(v1) < LIARA_VERSION_MINOR(v2);
    }
    return v1 < v2;
}

/**
 * @deprecated This function is deprecated and will be removed in future versions. Use `liara_version_compare` instead.
 * @brief Inline function to compare two version numbers to determine if the first is less than or equal to the second,
 * with an option to ignore the patch version.
 *
 * This function compares two version numbers to determine if the first is less than or equal to the second. If the
 * `ignore_patch` parameter is set to a non-zero value, the comparison will only consider the major and minor version
 * components, ignoring the patch version.
 *
 * @param[in] v1 The first version number to compare.
 * @param[in] v2 The second version number to compare.
 * @param[in] ignore_patch If non-zero, ignore the patch version in the comparison.
 * @return Non-zero if the first version is considered less than or equal to the second based on the specified criteria,
 * zero otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and will be removed in future versions. Use liara_version_compare instead.")

static inline bool liara_version_lte(const uint32_t v1, const uint32_t v2, const bool ignore_patch) {
    return (liara_version_lt(v1, v2, ignore_patch) || liara_version_eq(v1, v2, ignore_patch)) != 0;
}

/**
 * @deprecated This function is deprecated and will be removed in future versions. Use `liara_version_compare` instead.
 * @brief Inline function to compare two version numbers to determine if the first is greater than the second, with an
 * option to ignore the patch version.
 *
 * This function compares two version numbers to determine if the first is greater than the second. If the
 * `ignore_patch` parameter is set to a non-zero value, the comparison will only consider the major and minor version
 * components, ignoring the patch version.
 *
 * @param[in] v1 The first version number to compare.
 * @param[in] v2 The second version number to compare.
 * @param[in] ignore_patch If non-zero, ignore the patch version in the comparison.
 * @return Non-zero if the first version is considered greater than the second based on the specified criteria, zero
 * otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and will be removed in future versions. Use liara_version_compare instead.")

static inline bool liara_version_gt(const uint32_t v1, const uint32_t v2, const bool ignore_patch) {
    return !liara_version_lte(v1, v2, ignore_patch) != 0;
}

/**
 * @deprecated This function is deprecated and will be removed in future versions. Use `liara_version_compare` instead.
 * @brief Inline function to compare two version numbers to determine if the first is greater than or equal to the
 * second, with an option to ignore the patch version.
 *
 * This function compares two version numbers to determine if the first is greater than or equal to the second. If the
 * `ignore_patch` parameter is set to a non-zero value, the comparison will only consider the major and minor version
 * components, ignoring the patch version.
 *
 * @param[in] v1 The first version number to compare.
 * @param[in] v2 The second version number to compare.
 * @param[in] ignore_patch If non-zero, ignore the patch version in the comparison.
 * @return Non-zero if the first version is considered greater than or equal to the second based on the specified
 * criteria, zero otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and will be removed in future versions. Use liara_version_compare instead.")

static inline bool liara_version_gte(const uint32_t v1, const uint32_t v2, const bool ignore_patch) {
    return !liara_version_lt(v1, v2, ignore_patch) != 0;
}

/**
 * @brief Inline function to compare two version numbers and return a signed integer indicating their relative order,
 * with an option to ignore the patch version.
 *
 * This function compares two version numbers and returns a signed integer indicating their relative order:
 * - A negative value if the first version is less than the second.
 * - Zero if the versions are equal.
 * - A positive value if the first version is greater than the second.
 *
 * If the `ignore_patch` parameter is set to a non-zero value, the comparison will only consider the major and minor
 * version components, ignoring the patch version.
 *
 * @param[in] v1 The first version number to compare.
 * @param[in] v2 The second version number to compare.
 * @param[in] ignore_patch If non-zero, ignore the patch version in the comparison.
 * @return A signed integer indicating the relative order of the versions.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline int8_t liara_version_compare(const uint32_t v1, const uint32_t v2, const bool ignore_patch) {
    uint32_t a = v1;
    uint32_t b = v2;

    // If ignoring patch, shift both versions right by the number of bits allocated for the patch version
    if (ignore_patch) {
        a >>= LIARA_VERSION_PATCH_BITS;
        b >>= LIARA_VERSION_PATCH_BITS;
    }

    return LIARA_STATIC_CAST(int8_t, (LIARA_STATIC_CAST(int8_t, a > b) - LIARA_STATIC_CAST(int8_t, a < b)));
}

#ifdef __cplusplus
}
#endif
