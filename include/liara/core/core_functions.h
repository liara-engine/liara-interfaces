/**
 * @file core_functions.h
 * @brief The liara_core_* entry points expressed as data, for a host that resolves them at run time.
 */

#pragma once

#include <liara/core/core.h>

/**
 * @brief Every entry point of the `liara_core_` namespace, as an X-macro list.
 */
#define LIARA_CORE_FUNCTIONS(X, prefix)                        \
    X(prefix, info, const liara_module_info_t*, (void))        \
    X(prefix, abi_version, uint32_t, (void))                   \
    X(prefix, create, liara_result_t, (liara_core_handle_t**)) \
    X(prefix, destroy, void, (liara_core_handle_t*))           \
    X(prefix, update, void, (liara_core_handle_t*, float))     \
    X(prefix, get_render_packet, liara_result_t, (const liara_core_handle_t*, liara_render_packet_t*))

/** The library name stem, without platform prefix or extension. */
#define LIARA_CORE_LIBRARY_STEM "liara_core"
