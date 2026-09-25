#pragma once

#include <liara/internal/portability.h>
#include <liara/platform/platform_export.h>

#ifndef __cplusplus
    #include <stdbool.h>
#endif
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

LIARA_TYPEDEF(struct liara_module_info_t, liara_module_info_t);
LIARA_TYPEDEF(int32_t, liara_result_t);

/**
 * @brief Opaque structure representing a Liara platform instance.
 *
 * The actual implementation details are hidden from the user, and the structure should only be manipulated through
 * the provided API functions.
 */
LIARA_TYPEDEF(struct liara_platform_t, liara_platform_handle_t);

/**
 * @brief Everything `liara_platform_create` needs in order to build an instance.
 */
LIARA_TYPEDEF_STRUCT(liara_platform_create_info_t,
                     uint32_t struct_version; /**< Version of the liara_platform_create_info_t structure. */
);

#define LIARA_PLATFORM_CREATE_INFO_VERSION 1U /**< Current version of liara_platform_create_info_t. */

/**
 * @brief Returns information about the Liara platform module.
 *
 * This function returns a pointer to a `liara_module_info_t` structure containing information about the Liara platform
 * module, including its version, ABI version, and name. The returned pointer is valid for the lifetime of the program
 * and should not be modified or freed by the caller.
 *
 * @return A pointer to a `liara_module_info_t` structure containing information about the Liara platform module.
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

/**
 * @brief Creates a new Liara platform instance.
 *
 * @param[in] create_info Describes the instance to build. It may not be null.
 * @param[out] out_platform Receives the new instance. Untouched unless the call succeeds.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The platform was created successfully.
 * - `LIARA_RESULT_NULL_POINTER`: `create_info` is null.
 * - `LIARA_RESULT_VERSION_MISMATCH`: `create_info->struct_version` is not a version this module understands.
 * - `LIARA_RESULT_OUT_OF_MEMORY`: The instance could not be allocated.
 *
 * @threadsafety This function is thread-safe. @endthreadsafety
 */
LIARA_PLATFORM_API liara_result_t liara_platform_create(const liara_platform_create_info_t* create_info,
                                                        liara_platform_handle_t** out_platform);

/**
 * @brief Destroys a Liara platform instance.
 *
 * @param[in] platform The instance to destroy. A null pointer is a no-op, as it is for `free`. Passing a handle that
 *            was already destroyed is undefined.
 *
 * @threadsafety This function is not thread-safe with respect to the handle it destroys. @endthreadsafety
 */
LIARA_PLATFORM_API void liara_platform_destroy(liara_platform_handle_t* platform);

/**
 * @brief Installs the process's handlers for the signals caught by Liara, for now only the user-requested stop signals.
 *
 * The set is fixed and not selectable. It is composed of:
 * - `quit_requested`: `SIGINT` and `SIGTERM` under POSIX, and the console control events
 * `CTRL_C_EVENT`, `CTRL_BREAK_EVENT` and `CTRL_CLOSE_EVENT` under Win32.
 *
 * This list can be extended in the future.
 *
 * Installation is process-global and idempotent: the handlers are installed at most once per process whatever number
 * of platform instances exist, and a second call succeeds without doing anything. It requires no window, so a headless
 * tool may use the module for shutdown alone.
 *
 * The handler writes a flag and does nothing else, no callback crosses this boundary.
 *
 * @param[in] platform The instance to associate with the handlers.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The handlers are installed.
 * - `LIARA_RESULT_NULL_POINTER`: `platform` is null.
 * - `LIARA_RESULT_FAILED`: The operating system refused to install a handler.
 *
 * @threadsafety This function is thread-safe. @endthreadsafety
 */
LIARA_PLATFORM_API liara_result_t liara_platform_install_signal_handlers(liara_platform_handle_t* platform);

/**
 * @brief Reports whether the user has asked the process to stop.
 *
 * The answer merges a process-global flag written by the signal handler, with the window's close button, which is
 * per-instance.
 *
 * The flag is sticky: once true it stays true for the life of the process. An entry point to clear it would let a host
 * cancel a quit, and it is deliberately absent until a consumer asks for it.
 *
 * @param[in] platform The instance to interrogate. Passing null is a programming error.
 *
 * @return `true` once a stop has been requested, `false` otherwise.
 *
 * @threadsafety This function is thread-safe. @endthreadsafety
 */
LIARA_PLATFORM_API bool liara_platform_quit_requested(const liara_platform_handle_t* platform);

/**
 * @brief Reads the module's monotonic clock, in nanoseconds.
 *
 * The origin is unspecified: only differences between two readings are meaningful, never the value itself. The
 * value never decreases, as observed from any thread. The tick rate is not promised to be constant, so this clock
 * measures durations and is not a frequency reference. Two readings taken close together may return the same value,
 * so a frame delta may legitimately be zero. Behaviour across a system suspend is unspecified.
 *
 * This clock is not a wall clock, do not use it to measure absolute time.
 *
 * @return The current reading of the monotonic clock, in nanoseconds.
 *
 * @see liara_platform_time_wall_ns for a wall-clock.
 *
 * @threadsafety This function is thread-safe. @endthreadsafety
 */
LIARA_PLATFORM_API uint64_t liara_platform_time_now_ns(void);

/**
 * @brief Reads the module's wall clock, in nanoseconds.
 *
 * The origin is the Unix epoch, 1970-01-01T00:00:00Z. The value may jump forward or backward at any time, for example
 * due to NTP adjustments or manual clock changes by the administrator. Its representable range, as a signed 64-bit
 * count of nanoseconds, ends in the year 2262.
 *
 * The value is UTC as the operating system reports it: no local time, no timezone, and no promise about leap
 * seconds. Its granularity is typically coarser than the monotonic counter's, and is likewise unspecified.
 *
 * This clock is not monotonic, do not use it to measure durations.
 *
 * @return The current wall-clock time, in nanoseconds since the Unix epoch.
 *
 * @see liara_platform_time_now_ns for a monotonic clock.
 *
 * @threadsafety This function is thread-safe. @endthreadsafety
 */
LIARA_PLATFORM_API int64_t liara_platform_time_wall_ns(void);

/**
 * @brief Blocks until the monotonic clock reaches a deadline.
 *
 * The deadline is a value of the monotonic clock returned by `liara_platform_time_now_ns`, not a wall-clock
 * timestamp: passing a value from `liara_platform_time_wall_ns` sleeps for roughly fifty-six years. Overshoot past
 * the deadline is unbounded and not promised to be small. A deadline that has already passed returns immediately,
 * with no promise that the call yields the thread. A signal does not cut the call short.
 *
 * @param[in] deadline_ns The monotonic-clock value to sleep until.
 *
 * @threadsafety This function is thread-safe. @endthreadsafety
 */
LIARA_PLATFORM_API void liara_platform_time_sleep_until_ns(uint64_t deadline_ns);

/**
 * @brief Returns the resolution of the monotonic clock, in nanoseconds.
 *
 * The resolution is the smallest possible difference between two readings of the monotonic clock.
 *
 * @return The resolution of the monotonic clock, in nanoseconds.
 *
 * @threadsafety This function is thread-safe. @endthreadsafety
 */
LIARA_PLATFORM_API uint64_t liara_platform_time_resolution_ns(void);

#ifdef __cplusplus
}
#endif
