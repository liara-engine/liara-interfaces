/**
 * @file version.h
 * @brief Liara interface version definitions.
 *
 * This header defines the versioning scheme for the Liara interface, including macros and inline functions
 * for creating and extracting version numbers.
 *
 * The version is represented as a 32-bit unsigned integer, where:
 * - The major version occupies bits 22-31 (10 bits).
 * - The minor version occupies bits 12-21 (10 bits).
 * - The patch version occupies bits 0-11 (12 bits).
 *
 * This allows for a maximum major version of 1023, a maximum minor version of 1023, and a maximum patch version of 4095.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Liara interface version components.
 *
 * These constants define the major, minor, and patch version numbers for the Liara interface.
 * They can be updated as needed to reflect changes in the interface.
 */
enum {
    /** Liara interface major version number. Update this when making incompatible API changes. */
    LIARA_INTERFACE_VERSION_MAJOR = 0,
    /** Liara interface minor version number. Update this when adding functionality in a backward-compatible manner. If there is any doubt on the backward compatibility of a change, you must update the major version instead. */
    LIARA_INTERFACE_VERSION_MINOR = 0,
    /** Liara interface patch version number. Update this when adding documentation, comments, whitespace, or other non-functional changes. */
    LIARA_INTERFACE_VERSION_PATCH = 0,
};

/**
 * @brief Create a version number from major, minor, and patch components.
 *
 * This macro combines the major, minor, and patch version numbers into a single 32-bit unsigned integer.
 *
 * @param major The major version number (0-1023).
 * @param minor The minor version number (0-1023).
 * @param patch The patch version number (0-4095).
 * @return A 32-bit unsigned integer representing the combined version.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state.
 */
#define LIARA_MAKE_VERSION(major, minor, patch) \
    (((uint32_t)(major) << 22) | ((uint32_t)(minor) << 12) | (uint32_t)(patch))

/**
 * @brief Inline function to create a version number from major, minor, and patch components.
 *
 * This function is a wrapper around the LIARA_MAKE_VERSION macro for other languages that may not support macros
 * or for better type safety in C. It combines the major, minor, and patch version numbers into a single 32-bit unsigned integer.
 *
 * @param major The major version number (0-1023).
 * @param minor The minor version number (0-1023).
 * @param patch The patch version number (0-4095).
 * @return A 32-bit unsigned integer representing the combined version.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state.
 */
static inline uint32_t liara_make_version(uint32_t major, uint32_t minor, uint32_t patch) { return LIARA_MAKE_VERSION(major, minor, patch); }

/**
 * @brief Extract the major version component from a combined version number.
 *
 * This macro extracts the major version number from a combined version number by shifting and masking.
 *
 * @param v The combined version number from which to extract the major version.
 * @return The major version number (0-1023).
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state.
 */
#define LIARA_VERSION_MAJOR(v) (((uint32_t)(v) >> 22) & 0x3FFu)

/**
 * @brief Extract the minor version component from a combined version number.
 *
 * This macro extracts the minor version number from a combined version number by shifting and masking.
 *
 * @param v The combined version number from which to extract the minor version.
 * @return The minor version number (0-1023).
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state.
 */
#define LIARA_VERSION_MINOR(v) (((uint32_t)(v) >> 12) & 0x3FFu)

/**
 * @brief Extract the patch version component from a combined version number.
 *
 * This macro extracts the patch version number from a combined version number by masking.
 *
 * @param v The combined version number from which to extract the patch version.
 * @return The patch version number (0-4095).
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state.
 */
#define LIARA_VERSION_PATCH(v) ((uint32_t)(v) & 0xFFFu)

/**
 * @brief Inline function to extract the major version component from a combined version number.
 *
 * This function is a wrapper around the LIARA_VERSION_MAJOR macro for other languages that may not support macros
 * or for better type safety in C. It extracts the major version number from a combined version number by shifting and masking.
 *
 * @param v The combined version number from which to extract the major version.
 * @return The major version number (0-1023).
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state.
 */
static inline uint32_t liara_version_major(uint32_t v) { return LIARA_VERSION_MAJOR(v); }

/**
 * @brief Inline function to extract the minor version component from a combined version number.
 *
 * This function is a wrapper around the LIARA_VERSION_MINOR macro for other languages that may not support macros
 * or for better type safety in C. It extracts the minor version number from a combined version number by shifting and masking.
 *
 * @param v The combined version number from which to extract the minor version.
 * @return The minor version number (0-1023).
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state.
 */
static inline uint32_t liara_version_minor(uint32_t v) { return LIARA_VERSION_MINOR(v); }

/**
 * @brief Inline function to extract the patch version component from a combined version number.
 *
 * This function is a wrapper around the LIARA_VERSION_PATCH macro for other languages that may not support macros
 * or for better type safety in C. It extracts the patch version number from a combined version number by masking.
 *
 * @param v The combined version number from which to extract the patch version.
 * @return The patch version number (0-4095).
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state.
 */
static inline uint32_t liara_version_patch(uint32_t v) { return LIARA_VERSION_PATCH(v); }

/**
 * @brief The current version of the Liara interface.
 *
 * This macro defines the current version of the Liara interface by combining the major, minor, and patch version numbers defined above.
 * It can be used to check for compatibility with specific versions of the interface.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state.
 */
#define LIARA_INTERFACE_VERSION \
    LIARA_MAKE_VERSION( \
        LIARA_INTERFACE_VERSION_MAJOR, \
        LIARA_INTERFACE_VERSION_MINOR, \
        LIARA_INTERFACE_VERSION_PATCH)

/**
 * @brief Inline function to get the current version of the Liara interface.
 *
 * This function is a wrapper around the LIARA_INTERFACE_VERSION macro for other languages that may not support macros
 * or for better type safety in C. It returns the current version of the Liara interface by combining the major, minor, and patch version numbers defined above.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara interface.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state.
 */
static inline uint32_t liara_interface_version(void) { return LIARA_INTERFACE_VERSION; }

#ifdef __cplusplus
}
#endif
