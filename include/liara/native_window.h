/**
 * @file native_window.h
 * @brief A window as the operating system's window system knows it: enough to create a Vulkan surface on it.
 */

#pragma once

#include <liara/internal/portability.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Which member of `liara_native_window_t.handle` is valid. One of the `LIARA_NATIVE_WINDOW_KIND_*` values.
 *
 * A `uint32_t` rather than the enum below, because an enum's size is implementation-defined.
 */
LIARA_TYPEDEF(uint32_t, liara_native_window_kind_t);

/**
 * @brief The values of `liara_native_window_kind_t`.
 *
 * New kinds are appended in minor versions and never renumbered. A consumer meeting a kind it does not know treats
 * the window as unusable, exactly as it treats `LIARA_NATIVE_WINDOW_KIND_NONE`.
 */
enum liara_native_window_kinds {
    LIARA_NATIVE_WINDOW_KIND_NONE = 0,    /**< No native window, e.g. SDL's offscreen driver. `handle` holds nothing. */
    LIARA_NATIVE_WINDOW_KIND_WIN32 = 1,   /**< `handle.win32` is valid. */
    LIARA_NATIVE_WINDOW_KIND_XLIB = 2,    /**< `handle.xlib` is valid. */
    LIARA_NATIVE_WINDOW_KIND_WAYLAND = 3, /**< `handle.wayland` is valid. */
};

/**
 * @brief A native window, tagged by its kind.
 *
 * Each member of `handle` carries exactly the fields of the matching Vulkan surface create-info, under the same names:
 * `VkWin32SurfaceCreateInfoKHR`, `VkXlibSurfaceCreateInfoKHR`, `VkWaylandSurfaceCreateInfoKHR`. A consumer reads only
 * the member `kind` names. `padding` fixes the struct at 40 bytes on 32- and 64-bit targets, so a kind added later
 * fits inside it without moving anything.
 *
 * The pointers are borrowed from whoever produced the struct, and stay valid as long as that producer says; for a
 * window from `liara-platform`, until the window is destroyed.
 *
 * @threadsafety Plain data, with no thread-safety concerns of its own. @endthreadsafety
 */
LIARA_TYPEDEF_STRUCT(liara_native_window_t,
                     liara_native_window_kind_t kind; /**< Which member of `handle` is valid. */
                     uint32_t reserved;               /**< Explicit padding. Zero. */
                     union {
                         struct {
                             void* hinstance; /**< `HINSTANCE`. */
                             void* hwnd;      /**< `HWND`. */
                         } win32;             /**< Valid when `kind` is `LIARA_NATIVE_WINDOW_KIND_WIN32`. */
                         struct {
                             void* dpy;       /**< Xlib `Display*`. */
                             uint64_t window; /**< Xlib `Window`, an XID. */
                         } xlib;              /**< Valid when `kind` is `LIARA_NATIVE_WINDOW_KIND_XLIB`. */
                         struct {
                             void* display; /**< `struct wl_display*`. */
                             void* surface; /**< `struct wl_surface*`. */
                         } wayland;         /**< Valid when `kind` is `LIARA_NATIVE_WINDOW_KIND_WAYLAND`. */
                         uint64_t padding[4]; /**< Fixes the size of the union. Never read. */
                     } handle; /**< The window itself, per `kind`. */
);

#ifdef __cplusplus
}
#endif
