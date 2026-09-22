/**
 * @file renderer_functions.h
 * @brief The liara_renderer_* entry points expressed as data, for a host that resolves them at run time.
 */

#pragma once

#include <liara/renderer/renderer.h>

/**
 * @brief Every entry point of the `liara_renderer_` namespace, as an X-macro list.
 */
#define LIARA_RENDERER_FUNCTIONS(X, prefix)                        \
    X(prefix, info, const liara_module_info_t*, (void))            \
    X(prefix, abi_version, uint32_t, (void))                       \
    X(prefix, create, liara_result_t, (liara_renderer_handle_t**)) \
    X(prefix, destroy, void, (liara_renderer_handle_t*))           \
    X(prefix, submit_frame, liara_result_t, (liara_renderer_handle_t*, const liara_render_packet_t*))

/** The library name stem, without platform prefix or extension. */
#define LIARA_RENDERER_LIBRARY_STEM "liara_renderer"
