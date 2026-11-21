/**
 * @file packet.h
 * @brief Render packet structures: the plain-old-data snapshot the core hands to the renderer each frame.
 *
 * A packet is a list of views. Each view names the target it draws into, the part of that target it covers, the colour
 * it clears that part to, and the drawables it draws there. A drawable references a mesh the renderer was given
 * beforehand through `liara_renderer_mesh_upload`; the packet itself carries no geometry.
 *
 * The packet's `struct_version` covers every structure it reaches: views and drawables carry no version of their own,
 * since the stride of an array cannot depend on a field of its elements.
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */

#pragma once

#include <liara/internal/portability.h>
#include <liara/mesh.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Identifies what a view draws into.
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
LIARA_TYPEDEF(uint64_t, liara_render_target_id_t);

/**
 * @brief Never a valid target. A view carrying it is refused.
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
#define LIARA_RENDER_TARGET_NONE LIARA_STATIC_CAST(liara_render_target_id_t, 0)

/**
 * @brief The window the renderer presents to, the only target in this version.
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
#define LIARA_RENDER_TARGET_WINDOW LIARA_STATIC_CAST(liara_render_target_id_t, 1)

/**
 * @brief The part of a target a view covers, as fractions of the target's size.
 *
 * The origin is the target's top-left corner and y goes down, as in window coordinates. Every component lies in
 * [0, 1], and so do `x + width` and `y + height`. Fractions rather than pixels, since the producer of a packet does not
 * know the target's size, nor that it changes.
 *
 * @threadsafety Plain data, with no thread-safety concerns of its own. @endthreadsafety
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
LIARA_TYPEDEF_STRUCT(liara_render_viewport_t, float x; /**< Left edge. */
                     float y;                          /**< Top edge. */
                     float width;                      /**< Horizontal extent. */
                     float height;                     /**< Vertical extent. */
);

/**
 * @brief One thing a view draws.
 *
 * It carries the mesh and nothing else: a transform and a material arrive with the versions that need them.
 *
 * @threadsafety Plain data, with no thread-safety concerns of its own. @endthreadsafety
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
LIARA_TYPEDEF_STRUCT(liara_render_drawable_t,
                     /**
                      * The mesh to draw, as uploaded through `liara_renderer_mesh_upload`. Never
                      * `LIARA_MESH_ID_NONE`. A mesh the renderer does not know is skipped, with a warning.
                      */
                     liara_mesh_id_t mesh;);

/**
 * @brief What one view draws, and where.
 *
 * Views are drawn in the packet's order, each over the previous ones. A view first clears its rectangle to
 * `clear_color`, then draws its drawables in order. A view targeting something the renderer does not know is skipped,
 * with a warning.
 *
 * @threadsafety Plain data, with no thread-safety concerns of its own. @endthreadsafety
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
LIARA_TYPEDEF_STRUCT(liara_render_view_t, liara_render_target_id_t target; /**< Never LIARA_RENDER_TARGET_NONE. */
                     liara_render_viewport_t viewport;
                     uint32_t clear_color;                     /**< 0xAARRGGBB, sRGB-encoded, alpha ignored. */
                     uint32_t reserved;                        /**< Explicit padding. Set to zero. */
                     const liara_render_drawable_t* drawables; /**< drawable_count entries, or null when it is 0. */
                     size_t drawable_count;                    /**< Number of entries in `drawables`. */
);

/**
 * @brief One frame's worth of views, extracted by the core and submitted to the renderer.
 *
 * @threadsafety Plain data, with no thread-safety concerns of its own. The lifetime of every pointer in the packet is
 *               documented by whichever function produces it (see `liara_core_get_render_packet`). @endthreadsafety
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
LIARA_TYPEDEF_STRUCT(liara_render_packet_t,
                     uint32_t struct_version; /**< LIARA_RENDER_PACKET_VERSION, which covers the views and drawables. */
                     uint32_t reserved;       /**< Explicit padding. Set to zero. */
                     const liara_render_view_t* views; /**< view_count entries, or null when it is 0. */
                     size_t view_count;                /**< Number of entries in `views`. */
);

/**
 * @brief Current version of liara_render_packet_t, and of every structure it reaches.
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
#define LIARA_RENDER_PACKET_VERSION 2U

#ifdef __cplusplus
}
#endif
