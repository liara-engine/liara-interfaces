/**
 * @file abi_version.h
 * @brief Liara ABI version definition and compatibility rules.
 */

#pragma once

#include <liara/private_utils.h>
#include <liara/version.h>
#include <liara/config.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Liara ABI interface version components.
 *
 * These constants define the major, minor, and patch version numbers for the Liara ABI interface.
 * They can be updated as needed to reflect changes in the interface.
 */
enum {
    /** Liara ABI major version number. Update this when making incompatible API changes. */
    LIARA_ABI_VERSION_MAJOR = LIARA_PRIVATE_CMAKE_VERSION_MAJOR,
    /** Liara ABI minor version number. Update this when adding functionality in a backward-compatible manner. If there
     * is any doubt on the backward compatibility of a change, you must update the major version instead. */
    LIARA_ABI_VERSION_MINOR = LIARA_PRIVATE_CMAKE_VERSION_MINOR,
    /** Liara ABI patch version number. Update this when adding documentation, comments, whitespace, or other
     * non-functional changes. */
    LIARA_ABI_VERSION_PATCH = LIARA_PRIVATE_CMAKE_VERSION_PATCH,
};

LIARA_STATIC_ASSERT(LIARA_ABI_VERSION_MAJOR <= LIARA_VERSION_MAJOR_MASK, "Major version exceeds maximum value");
LIARA_STATIC_ASSERT(LIARA_ABI_VERSION_MINOR <= LIARA_VERSION_MINOR_MASK, "Minor version exceeds maximum value");
LIARA_STATIC_ASSERT(LIARA_ABI_VERSION_PATCH <= LIARA_VERSION_PATCH_MASK, "Patch version exceeds maximum value");

/**
 * @brief The current version of the Liara ABI interface.
 *
 * This macro defines the current version of the Liara ABI interface by combining the major, minor, and patch version
 * numbers defined above. It can be used to check for compatibility with specific versions of the interface.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#define LIARA_ABI_VERSION \
    (uint32_t)LIARA_MAKE_VERSION_UNSAFE(LIARA_ABI_VERSION_MAJOR, LIARA_ABI_VERSION_MINOR, LIARA_ABI_VERSION_PATCH)

/**
 * @brief Inline function to get the current version of the Liara ABI interface.
 *
 * This function is a wrapper around the LIARA_ABI_VERSION macro for other languages that may not support macros
 * or for better type safety in C. It returns the current version of the Liara ABI interface by combining the major,
 * minor, and patch version numbers defined above.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara ABI interface.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline uint32_t liara_abi_version(void) { return LIARA_ABI_VERSION; }

/**
 * @brief Inline function to check if a version number satisfies the requirements of another version number.
 *
 * This function checks if a version number satisfies the requirements of another version number based on their major
 * and minor version components. Satisfaction is defined as having the same major version and a minor version that is
 * greater than or equal to the specified version's minor version for post 0.1 versions. For pre-0.1 versions,
 * satisfaction is defined as having the same major, minor, and patch versions.
 *
 * @warning This function does not respect the strict semantic versioning rules for pre-0.1 versions.
 *          It's the caller's responsibility to be aware of the versioning scheme used and to ensure that compatibility
 *          checks are performed appropriately for the specific versions in use.
 *
 * @param required[in] The required version number to satisfy.
 * @param available[in] The available version number to check against the required version.
 * @return True if the available version satisfies the required version, false otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline bool liara_version_satisfies(const uint32_t required, const uint32_t available) {
    if (LIARA_VERSION_MAJOR(required) != LIARA_VERSION_MAJOR(available)) { return false; }
    if (LIARA_VERSION_MAJOR(required) == 0 && LIARA_VERSION_MINOR(required) == 0) { return required == available; }
    return LIARA_VERSION_MINOR(available) >= LIARA_VERSION_MINOR(required);
}

/**
 * @brief Inline function to check if the current ABI version satisfies the requirements of a specified version.
 *
 * This function checks if the current ABI version satisfies the requirements of a specified version.
 * Satisfaction is defined as having the same major version and a minor version that is greater than or equal to
 * the specified version's minor version for post 0.1 versions. For pre-0.1 versions, satisfaction is defined as
 * having the same major, minor, and patch versions.
 *
 * @warning This function does not respect the strict semantic versioning rules for pre-0.1 versions.
 *          It's the caller's responsibility to be aware of the versioning scheme used and to ensure that compatibility
 *          checks are performed appropriately for the specific versions in use.
 *
 * @param required_version[in] The required version number to satisfy.
 * @return True if the current ABI version satisfies the requirements of the specified version, false otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline bool liara_abi_version_satisfies(const uint32_t required_version) {
    return liara_version_satisfies(required_version, LIARA_ABI_VERSION);
}

#ifdef __cplusplus
}
#endif
