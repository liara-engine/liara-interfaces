/**
 * @file platform_functions.h
 * @brief The liara_platform_* entry points expressed as data, for a host that resolves them at run time.
 */

#pragma once

#include <liara/platform/platform.h>

/**
 * @brief Every entry point of the `liara_platform_` namespace, as an X-macro list.
 */
#define LIARA_PLATFORM_FUNCTIONS(X, prefix)                                                             \
    X(prefix, info, const liara_module_info_t*, (void))                                                 \
    X(prefix, abi_version, uint32_t, (void))                                                            \
    X(prefix, create, liara_result_t, (const liara_platform_create_info_t*, liara_platform_handle_t**)) \
    X(prefix, destroy, void, (liara_platform_handle_t*))                                                \
    X(prefix, install_signal_handlers, liara_result_t, (liara_platform_handle_t*))                      \
    X(prefix, quit_requested, bool, (const liara_platform_handle_t*))                                   \
    X(prefix, time_now_ns, uint64_t, (void))                                                            \
    X(prefix, time_wall_ns, int64_t, (void))                                                            \
    X(prefix, time_sleep_until_ns, void, (uint64_t))                                                    \
    X(prefix, time_resolution_ns, uint64_t, (void))

/** The library name stem, without platform prefix or extension. */
#define LIARA_PLATFORM_LIBRARY_STEM "liara_platform"
