/**
 * @file packet.h
 * @brief Render packet structures: the plain-old-data snapshot the core hands to the renderer
 *        each frame.
 */

#pragma once

#include <liara/internal/portability.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A single drawable submitted to the renderer as part of a render packet.
 * @threadsafety This type has no thread-safety concerns of its own; it is plain data.
 */
LIARA_TYPEDEF_STRUCT(liara_render_drawable_t,
                     float x;        /**< Position in the render target's grid space, [0, grid_width). */
                     float y;        /**< Position in the render target's grid space, [0, grid_height). */
                     uint32_t color; /**< Color, 0xAARRGGBB. */
);

/**
 * @brief One frame's worth of drawable data, extracted by the core and submitted to the renderer.
 *
 * @threadsafety This type has no thread-safety concerns of its own; it is plain data. The
 *               lifetime of the `drawables` pointer is documented by whichever function produces
 *               the packet (see `liara_core_get_render_packet`).
 */
LIARA_TYPEDEF_STRUCT(
    liara_render_packet_t, uint32_t struct_version; /**< Version of this structure. See LIARA_RENDER_PACKET_VERSION. */
    uint16_t grid_width;                            /**< Width of the render target, in grid cells. */
    uint16_t grid_height;                           /**< Height of the render target, in grid cells. */
    uint32_t background_color;                      /**< Color used for cells with no drawable, 0xAARRGGBB. */
    const liara_render_drawable_t* drawables;       /**< Caller-owned array of drawables; see the note above. */
    size_t drawable_count;                          /**< Number of entries in `drawables`. */
);

/** Current version of the liara_render_packet_t structure. */
#define LIARA_RENDER_PACKET_VERSION 1U

#ifdef __cplusplus
}
#endif
