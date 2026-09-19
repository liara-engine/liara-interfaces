#pragma once

#include <liara/internal/portability.h>
#include <liara/platform/platform_export.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

LIARA_TYPEDEF(struct liara_module_info_t, liara_module_info_t);

/**
 * @brief Returns information about the Liara platform module.
 *
 * This function returns a pointer to a `liara_module_info_t` structure containing information about the Liara platform
 * module, including its version, ABI version, and name. The returned pointer is valid for the lifetime of the program
 * and should not be modified or freed by the caller.
 *
 * @return A pointer to a `liara_module_info_t` structure containing information about the Liara platform module.
 *
 * @note This is the whole of the platform namespace today. The module owns the window, the input devices, the OS
 *       signals and the clock, and none of that is implemented yet; the entry points for it are designed alongside
 *       their first implementation rather than ahead of it.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_PLATFORM_API const liara_module_info_t* liara_platform_info(void);

/**
 * @brief Returns the current version of the Liara platform ABI interface.
 *
 * This function returns the current version of the Liara platform ABI interface as a 32-bit unsigned integer. The
 * version is encoded using the same scheme as the Liara ABI version, with major, minor, and patch components.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara platform ABI interface.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_PLATFORM_API uint32_t liara_platform_abi_version(void);

#ifdef __cplusplus
}
#endif
