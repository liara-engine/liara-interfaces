/**
 * @file platform_functions.h
 * @brief The liara_platform_* entry points expressed as data, for a host that resolves them at run time.
 */

#pragma once

#include <liara/platform/platform.h>

/**
 * @brief Every entry point of the `liara_platform_` namespace, as an X-macro list.
 */
#define LIARA_PLATFORM_FUNCTIONS(X, prefix)             \
    X(prefix, info, const liara_module_info_t*, (void)) \
    X(prefix, abi_version, uint32_t, (void))

/** The library name stem, without platform prefix or extension. */
#define LIARA_PLATFORM_LIBRARY_STEM "liara_platform"
