/**
 * @file mesh.h
 * @brief Mesh identifiers and the CPU-side description of a mesh, shared by whatever produces mesh data and whatever
 *        consumes it.
 *
 * The renderer receives a mesh through `liara_renderer_mesh_upload`, keyed by an identifier its source chose, and a
 * render packet's drawables refer to it by that identifier. In v0.1 the host is the source; from v0.3, `liara-assets`.
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */

#pragma once

#include <liara/internal/portability.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Identifies a mesh, as chosen by whatever supplies its data.
 *
 * Opaque to everything but its source: a consumer compares identifiers and never reads meaning into their bits.
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
LIARA_TYPEDEF(uint64_t, liara_mesh_id_t);

/**
 * @brief Never a valid mesh. A drawable never carries it, and `liara_core_draw_mesh` takes it as "draw nothing".
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
#define LIARA_MESH_ID_NONE ((liara_mesh_id_t)0)

/**
 * @brief A mesh as separate streams of per-vertex data, the way glTF lays out its attributes.
 *
 * The topology is a list of triangles and nothing else: three indices per triangle when the mesh is indexed, three
 * consecutive vertices otherwise. Positions are normalized device coordinates, x to the right and y up, each in
 * [-1, 1]; z is carried and unused, since nothing has depth yet. Every pointer is borrowed for the call it is passed
 * to: the receiver copies the data during that call and keeps nothing of the caller's.
 *
 * @threadsafety Plain data, with no thread-safety concerns of its own. @endthreadsafety
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
LIARA_TYPEDEF_STRUCT(liara_mesh_data_t, uint32_t struct_version; /**< LIARA_MESH_DATA_VERSION. */
                     uint32_t reserved;      /**< Explicit padding. Set to zero; version 1 ignores it. */
                     const float* positions; /**< x, y, z per vertex: 3 * vertex_count floats. Never null. */
                     /**
                      * One colour per vertex, `0xAARRGGBB`, sRGB-encoded, alpha ignored; or null, for white.
                      */
                     const uint32_t* colors;
                     size_t vertex_count;     /**< Number of vertices, at least 1. */
                     const uint32_t* indices; /**< index_count entries, each below vertex_count, or null. */
                     size_t index_count;      /**< 0: not indexed, the vertices are taken three at a time. */
);

/**
 * @brief Current version of liara_mesh_data_t.
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
#define LIARA_MESH_DATA_VERSION 1U

#ifdef __cplusplus
}
#endif
