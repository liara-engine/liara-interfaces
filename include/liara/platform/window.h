/**
 * @file window.h
 * @brief The platform's windows: creation, destruction, size, and the native handle a renderer presents through.
 */

#pragma once

#include <liara/extent.h>
#include <liara/internal/portability.h>
#include <liara/native_window.h>
#include <liara/platform/platform.h>
#include <liara/platform/platform_export.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque structure representing one window of a Liara platform instance.
 *
 * A window belongs to the instance it was created from, and is destroyed before that instance is.
 */
LIARA_TYPEDEF(struct liara_platform_window_t, liara_platform_window_handle_t);

/**
 * @brief Everything `liara_platform_window_create` needs in order to open a window.
 *
 * Versioned: fields are appended in minor versions, and `struct_version` says which layout the caller compiled
 * against.
 */
LIARA_TYPEDEF_STRUCT(liara_platform_window_create_info_t,
                     uint32_t struct_version;         /**< LIARA_PLATFORM_WINDOW_CREATE_INFO_VERSION. */
                     liara_extent2d_t requested_size; /**< Logical units. A request the window manager may override. */
                     uint32_t reserved;               /**< Explicit padding. Set to zero; version 1 ignores it. */
                     const char* title;   /**< UTF-8, borrowed for the call. Null only when `title_length` is 0. */
                     size_t title_length; /**< Length of `title` in bytes. No terminator is required after it. */
);

#define LIARA_PLATFORM_WINDOW_CREATE_INFO_VERSION 1U /**< Current version of liara_platform_window_create_info_t. */

/**
 * @brief Opens a window.
 *
 * The size asked for is in the window system's logical units and is a request: a tiling window manager ignores it.
 * Every size the module reports back is in pixels. The window is always resizable. Its content is unspecified until
 * something presents to it.
 *
 * @param[in] platform The instance the window belongs to. It must outlive the window.
 * @param[in] create_info Describes the window. It may not be null.
 * @param[out] out_window Receives the new window. Untouched unless the call succeeds.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The window is open.
 * - `LIARA_RESULT_NULL_POINTER`: `platform` or `create_info` is null, or `title` is null while `title_length` is not
 *   zero.
 * - `LIARA_RESULT_VERSION_MISMATCH`: `create_info->struct_version` is not a version this module understands.
 * - `LIARA_RESULT_INVALID_ARGUMENT`: a component of `requested_size` is zero.
 * - `LIARA_RESULT_FAILED`: the window system could not be reached, or refused the window.
 * - `LIARA_RESULT_OUT_OF_MEMORY`: the window could not be allocated.
 *
 * @threadsafety Call from the process's main thread only. @endthreadsafety
 */
LIARA_PLATFORM_API liara_result_t liara_platform_window_create(liara_platform_handle_t* platform,
                                                               const liara_platform_window_create_info_t* create_info,
                                                               liara_platform_window_handle_t** out_window);

/**
 * @brief Destroys a window.
 *
 * Events still queued for the window are discarded with it, so no event returned afterwards names a destroyed window.
 *
 * @param[in] window The window to destroy. A null pointer is a no-op, as it is for `free`. Passing a window that was
 *            already destroyed is undefined.
 *
 * @threadsafety Call from the process's main thread only. @endthreadsafety
 */
LIARA_PLATFORM_API void liara_platform_window_destroy(liara_platform_window_handle_t* window);

/**
 * @brief Reads the window's current size, in pixels.
 *
 * A component may be zero, for instance while the window is minimised on some systems; a consumer sizing a swapchain
 * from it has to handle that.
 *
 * @param[in] window The window to measure.
 * @param[out] out_pixel_size Receives the size. Untouched unless the call succeeds.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: `out_pixel_size` holds the size.
 * - `LIARA_RESULT_NULL_POINTER`: `window` is null.
 * - `LIARA_RESULT_FAILED`: the window system could not answer.
 *
 * @threadsafety Call from the process's main thread only. @endthreadsafety
 */
LIARA_PLATFORM_API liara_result_t liara_platform_window_get_pixel_size(const liara_platform_window_handle_t* window,
                                                                       liara_extent2d_t* out_pixel_size);

/**
 * @brief Describes the window as the window system knows it, for a renderer to create its surface on.
 *
 * The pointers written into `out_native` stay valid until the window is destroyed. The call itself is main-thread
 * only; what may be done with its result is whatever Vulkan and the window system allow.
 *
 * @param[in] window The window to describe.
 * @param[out] out_native Receives the description. On `LIARA_RESULT_NOT_SUPPORTED`, its `kind` is
 *             `LIARA_NATIVE_WINDOW_KIND_NONE`.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: `out_native` describes the window.
 * - `LIARA_RESULT_NULL_POINTER`: `window` is null.
 * - `LIARA_RESULT_NOT_SUPPORTED`: the backend has no native window, as under SDL's offscreen driver.
 *
 * @threadsafety Call from the process's main thread only. @endthreadsafety
 */
LIARA_PLATFORM_API liara_result_t liara_platform_window_get_native(const liara_platform_window_handle_t* window,
                                                                   liara_native_window_t* out_native);

#ifdef __cplusplus
}
#endif
