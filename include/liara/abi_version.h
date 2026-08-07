/**
 * @file abi_version.h
 * @brief Liara ABI version definition and compatibility rules.
 */

#pragma once

#include <liara/config.h>
#include <liara/internal/portability.h>
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
 * @brief String representation of the Liara ABI version compatibility enumeration.
 *
 * This macro provides a string representation of the Liara ABI version compatibility enumeration values. It can be used
 * for logging, debugging, or displaying compatibility information to users.
 *
 * @param[in] compat The compatibility enumeration value to convert to a string.
 * @return A string literal representing the compatibility state.
 *
 * @threadsafety This macro is thread-safe as it does not modify any shared state. @endthreadsafety
 */
#define LIARA_VERSION_COMPAT_STR(compat)                              \
    ((compat) == LIARA_VERSION_COMPAT_EXACT          ? "EXACT"        \
     : (compat) == LIARA_VERSION_COMPAT_COMPATIBLE   ? "COMPATIBLE"   \
     : (compat) == LIARA_VERSION_COMPAT_DEGRADED     ? "DEGRADED"     \
     : (compat) == LIARA_VERSION_COMPAT_INCOMPATIBLE ? "INCOMPATIBLE" \
                                                     : "UNKNOWN")

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
LIARA_CONSTEXPR_FN uint32_t liara_abi_version(void) { return LIARA_ABI_VERSION; }

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

/**
 * @brief Inline function to get the string representation of a Liara ABI version compatibility enumeration value.
 *
 * This function is a wrapper around the LIARA_VERSION_COMPAT_STR macro for other languages that may not support macros
 * or for better type safety in C. It returns a string representation of the provided compatibility enumeration value.
 *
 * @param[in] compat The compatibility enumeration value to convert to a string.
 * @return A string literal representing the compatibility state.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
static inline const char* liara_version_compat_str(const liara_version_compat_t compat) {
    return LIARA_VERSION_COMPAT_STR(compat);
}

/**
 * @brief Inline function to determine the compatibility of a provided version with a required version.
 *
 * This function checks the compatibility of a provided version with a required version based on their major and minor
 * version components. It returns an enumeration value indicating the level of compatibility between the two versions.
 *
 * The rule, in order: identical versions are EXACT; differing majors are INCOMPATIBLE; a 0.0.x
 * version on either side demands exact equality and is otherwise INCOMPATIBLE; an older provided
 * minor is DEGRADED; anything else is COMPATIBLE.
 *
 * @param[in] provided The version being offered — typically a module's reported ABI version.
 * @param[in] required The version being asked for — typically the caller's LIARA_ABI_VERSION.
 * @return The compatibility state between the two versions.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_CONSTEXPR_FN liara_version_compat_t liara_version_provides(const uint32_t provided, const uint32_t required) {
    if (provided == required) { return LIARA_VERSION_COMPAT_EXACT; }
    if (LIARA_VERSION_MAJOR(provided) != LIARA_VERSION_MAJOR(required)) { return LIARA_VERSION_COMPAT_INCOMPATIBLE; }

    if ((LIARA_VERSION_MAJOR(provided) == 0U && LIARA_VERSION_MINOR(provided) == 0U)
        || (LIARA_VERSION_MAJOR(required) == 0U && LIARA_VERSION_MINOR(required) == 0U)) {
        return LIARA_VERSION_COMPAT_INCOMPATIBLE;
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
LIARA_CONSTEXPR_FN liara_version_compat_t liara_abi_is_compatible(const uint32_t module_abi) {
    return liara_version_provides(module_abi, LIARA_ABI_VERSION);
}

#ifdef __cplusplus
}
#endif
