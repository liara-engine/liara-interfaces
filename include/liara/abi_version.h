/**
 * @file abi_version.h
 * @brief Liara ABI version definition and compatibility rules.
 */

#pragma once

#include <liara/config.h>
#include <liara/private_utils.h>
#include <liara/version.h>

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
 * @brief Liara ABI version compatibility enumeration.
 *
 * This enumeration defines the possible compatibility states between different versions of the Liara ABI interface.
 * It can be used to determine whether a specific version of the interface is compatible with another version.
 */
typedef enum liara_version_compat {
    LIARA_VERSION_COMPAT_EXACT = 0,         // identical
    LIARA_VERSION_COMPAT_COMPATIBLE = 1,    // provider newer minor, fully usable
    LIARA_VERSION_COMPAT_DEGRADED = 2,      // provider older minor: some newer functions unavailable
    LIARA_VERSION_COMPAT_INCOMPATIBLE = 3,  // major mismatch, or 0.0.x inequality
} liara_version_compat_t;

/**
 * @brief The current version of the Liara ABI interface.
 *
 * This macro defines the current version of the Liara ABI interface by combining the major, minor, and patch version
 * numbers defined above. It can be used to check for compatibility with specific versions of the interface.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#define LIARA_ABI_VERSION \
    LIARA_STATIC_CAST(    \
        uint32_t,         \
        LIARA_MAKE_VERSION_UNSAFE(LIARA_ABI_VERSION_MAJOR, LIARA_ABI_VERSION_MINOR, LIARA_ABI_VERSION_PATCH))

/**
 * @brief String representation of the current version of the Liara ABI interface.
 *
 * This macro defines a string representation of the current version of the Liara ABI interface by combining the major,
 * minor, and patch version numbers defined above. It can be used for logging, debugging, or displaying version
 * information to users.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#define LIARA_ABI_VERSION_STR                         \
    LIARA_TOSTRING(LIARA_PRIVATE_CMAKE_VERSION_MAJOR) \
    "." LIARA_TOSTRING(LIARA_PRIVATE_CMAKE_VERSION_MINOR) "." LIARA_TOSTRING(LIARA_PRIVATE_CMAKE_VERSION_PATCH)

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
 * @brief Inline function to get the string representation of the current version of the Liara ABI interface.
 *
 * This function is a wrapper around the LIARA_ABI_VERSION_STR macro for other languages that may not support macros
 * or for better type safety in C. It returns a string representation of the current version of the Liara ABI interface
 * by combining the major, minor, and patch version numbers defined above.
 *
 * @return A string literal representing the current version of the Liara ABI interface.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline const char* liara_abi_version_str(void) { return LIARA_ABI_VERSION_STR; }

// TODO: Remove this function (marked as deprecated in 0.1.1)
/**
 * @deprecated This function is deprecated and will be removed in future versions. Use liara_version_provides() instead.
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
 * @param[in] required The required version number to satisfy.
 * @param[in] available The available version number to check against the required version.
 * @return True if the available version satisfies the required version, false otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED("Use liara_version_provides() instead")

static inline bool liara_version_satisfies(const uint32_t required, const uint32_t available) {
    if (LIARA_VERSION_MAJOR(required) != LIARA_VERSION_MAJOR(available)) { return false; }
    if (LIARA_VERSION_MAJOR(required) == 0 && LIARA_VERSION_MINOR(required) == 0) { return required == available; }
    return LIARA_VERSION_MINOR(available) >= LIARA_VERSION_MINOR(required);
}

// TODO: Remove this function (marked as deprecated in 0.1.1)
static inline liara_version_compat_t liara_abi_is_compatible(uint32_t module_abi);
/**
 * @deprecated This function is deprecated and will be removed in future versions. Use liara_abi_is_compatible()
 * instead.
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
 * @param[in] required_version The required version number to satisfy.
 * @return True if the current ABI version satisfies the requirements of the specified version, false otherwise.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED("Use liara_abi_is_compatible() instead")

static inline bool liara_abi_version_satisfies(const uint32_t required_version) {
    return liara_abi_is_compatible(required_version) != LIARA_VERSION_COMPAT_INCOMPATIBLE;
}

/**
 * @brief Inline function to determine the compatibility of a provided version with a required version.
 *
 * This function checks the compatibility of a provided version with a required version based on their major and minor
 * version components. It returns an enumeration value indicating the level of compatibility between the two versions.
 *
 * @param[in] provided The provided version number to check for compatibility.
 * @param[in] required The required version number to check against the provided version.
 * @return An enumeration value indicating the level of compatibility between the provided and required versions.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline liara_version_compat_t liara_version_provides(const uint32_t provided, const uint32_t required) {
    if (provided == required) { return LIARA_VERSION_COMPAT_EXACT; }
    if (LIARA_VERSION_MAJOR(provided) != LIARA_VERSION_MAJOR(required)) { return LIARA_VERSION_COMPAT_INCOMPATIBLE; }
    if (LIARA_VERSION_MAJOR(provided) == 0 && LIARA_VERSION_MINOR(provided) == 0) {
        return provided == required ? LIARA_VERSION_COMPAT_EXACT : LIARA_VERSION_COMPAT_INCOMPATIBLE;
    }
    if (LIARA_VERSION_MINOR(provided) < LIARA_VERSION_MINOR(required)) { return LIARA_VERSION_COMPAT_DEGRADED; }
    return LIARA_VERSION_COMPAT_COMPATIBLE;
}

/**
 * @brief Inline function to determine the compatibility of a provided ABI version with the current ABI version.
 *
 * This function checks the compatibility of a provided ABI version with the current ABI version based on their major
 * and minor version components. It returns an enumeration value indicating the level of compatibility between the two
 * versions.
 *
 * @param[in] module_abi The provided ABI version number to check for compatibility.
 * @return An enumeration value indicating the level of compatibility between the provided ABI version and the current
 *         ABI version.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline liara_version_compat_t liara_abi_is_compatible(const uint32_t module_abi) {
    return liara_version_provides(module_abi, LIARA_ABI_VERSION);
}

#ifdef __cplusplus
}
#endif
