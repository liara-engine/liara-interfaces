/**
 * @file extent.h
 * @brief A two-dimensional size, shared by every module that measures a surface.
 */

#pragma once

#include <liara/internal/portability.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A width and a height.
 *
 * The type carries no unit. Whatever holds an extent names it: `requested_size` is in the window system's logical
 * units, `pixel_size` in pixels.
 *
 * @threadsafety Plain data, with no thread-safety concerns of its own. @endthreadsafety
 */
LIARA_TYPEDEF_STRUCT(liara_extent2d_t, uint32_t width; /**< Horizontal size. */
                     uint32_t height;                  /**< Vertical size, in the same unit as `width`. */
);

#ifdef __cplusplus
}
#endif
