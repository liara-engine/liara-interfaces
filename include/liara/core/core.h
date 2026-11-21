#pragma once

#include <liara/core/core_export.h>
#include <liara/internal/portability.h>
#include <liara/mesh.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

LIARA_TYPEDEF(struct liara_module_info_t, liara_module_info_t);
LIARA_TYPEDEF(struct liara_render_packet_t, liara_render_packet_t);
LIARA_TYPEDEF(int32_t, liara_result_t);

/**
 * @brief Opaque structure representing a Liara core instance.
 *
 * This structure is used to encapsulate the internal state and resources associated with a Liara core. The actual
 * implementation details are hidden from the user, and the structure should only be manipulated through the provided
 * API functions.
 */
LIARA_TYPEDEF(struct liara_core_t, liara_core_handle_t);

/**
 * @brief Returns information about the Liara core module.
 *
 * This function returns a pointer to a `liara_module_info_t` structure containing information about the Liara core
 * module, including its version, ABI version, and name. The returned pointer is valid for the lifetime of the program
 * and should not be modified or freed by the caller.
 *
 * @return A pointer to a `liara_module_info_t` structure containing information about the Liara core module.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_CORE_API const liara_module_info_t* liara_core_info(void);

/**
 * @brief Returns the current version of the Liara core ABI interface.
 *
 * This function returns the current version of the Liara core ABI interface as a 32-bit unsigned integer. The
 * version is encoded using the same scheme as the Liara ABI version, with major, minor, and patch components.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara core ABI interface.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_CORE_API uint32_t liara_core_abi_version(void);

/**
 * @brief Creates a new Liara core instance.
 *
 * This function allocates and initializes a new Liara core instance. The created core is returned through the
 * `out_core` parameter. The caller is responsible for destroying the core using `liara_core_destroy`.
 *
 * @param[out] out_core A pointer to a pointer that will receive the newly created core instance.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The core was created successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `out_core` parameter is a null pointer.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with `liara_core_destroy` on
 * the same core instance. @endthreadsafety
 */
LIARA_CORE_API liara_result_t liara_core_create(liara_core_handle_t** out_core);

/**
 * @brief Destroys a Liara core instance.
 *
 * This function deallocates and cleans up the resources associated with a Liara core instance. After calling this
 * function, the core pointer must not be used again.
 *
 * @param[in] core_handle A pointer to the core instance to be destroyed. A null pointer is a no-op, as it is for
 *            `free`. Passing a handle that was already destroyed is undefined.
 *
 * @threadsafety This function is not thread-safe with respect to the handle it destroys. @endthreadsafety
 */
LIARA_CORE_API void liara_core_destroy(liara_core_handle_t* core_handle);

/**
 * @brief Advances the Liara core by one tick.
 *
 * This function performs a single update of the Liara core. The `delta_time` parameter specifies the time elapsed
 * since the last update, allowing for time-based calculations within the core.
 *
 * The core does not own the application loop and has no notion of real time: it advances only when asked, by exactly
 * the amount it is given. Calling it in a loop, deciding the delta and deciding when to stop are the host's job, for
 * the reasons in ADR 0003. A host that never calls this function has a core that never advances, which is what makes
 * a headless test or an editor stepping frame by frame possible without a special mode.
 *
 * @param[in] core_handle A pointer to the core instance to update. Ignored if `NULL` or already destroyed.
 * @param[in] delta_time The time elapsed since the last update, in seconds.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
LIARA_CORE_API void liara_core_update(liara_core_handle_t* core_handle, float delta_time);

/**
 * @brief Retrieves the render packet built during the core's most recent update.
 *
 * This is the core's half of the render packet pattern. It is the host's job to call this after `liara_core_update`
 * and to hand the packet to a renderer through `liara_renderer_submit_frame`. The core never calls the renderer
 * itself: the data path is always core, then host, then renderer.
 *
 * The packet holds one view, targeting `LIARA_RENDER_TARGET_WINDOW`, covering the whole target and cleared to the
 * core's background colour. That view has one drawable, referencing the mesh given to `liara_core_draw_mesh`, once the
 * host has named one and the core has been updated since; it has none otherwise.
 *
 * @param[in] core_handle A pointer to the core instance whose render packet is to be retrieved.
 * @param[out] out_packet A pointer to a `liara_render_packet_t` that will receive a copy of the packet.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The render packet was retrieved successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `core_handle` or `out_packet` parameter is a null pointer.
 * - `LIARA_RESULT_INVALID_STATE`: The core instance is in an invalid state (e.g., already destroyed).
 *
 * @warning Every pointer in the packet (`views`, and each view's `drawables`) refers to memory owned by the core, valid
 *          until the next call to `liara_core_update` or until `core_handle` is destroyed. The host must finish
 *          consuming the packet, typically by passing it to `liara_renderer_submit_frame`, before the next update,
 *          and must not keep any of those pointers across it.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that
 * modify the core's state. @endthreadsafety
 */
LIARA_CORE_API liara_result_t liara_core_get_render_packet(const liara_core_handle_t* core_handle,
                                                           liara_render_packet_t* out_packet);

/**
 * @brief Names the mesh the core's packets draw.
 *
 * From the next `liara_core_update` on, each packet's view carries one drawable referencing `mesh`, until another call
 * names another. `LIARA_MESH_ID_NONE` returns to no drawable, which is also the state before the first call. The core
 * does not know whether a renderer has that mesh, and does not try to: the identifier is the host's, and a renderer
 * skips a drawable whose mesh it lacks.
 *
 * @param[in] core_handle The core whose packets draw the mesh.
 * @param[in] mesh The mesh to draw, as uploaded to the renderer, or `LIARA_MESH_ID_NONE` for none.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The next update's packet draws `mesh`.
 * - `LIARA_RESULT_NULL_POINTER`: `core_handle` is null.
 * - `LIARA_RESULT_INVALID_STATE`: The core instance is in an invalid state.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that
 * modify the core's state. @endthreadsafety
 *
 * @warning Experimental. Part of the v0.1 render packet experiment. Expected to be replaced wholesale in ABI 2.0.0,
 *          when v0.2 designs the real packet from what this one taught. Do not extend it to avoid that rewrite.
 */
LIARA_CORE_API liara_result_t liara_core_draw_mesh(liara_core_handle_t* core_handle, liara_mesh_id_t mesh);

#ifdef __cplusplus
}
#endif
