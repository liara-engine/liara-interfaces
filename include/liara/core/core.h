#pragma once

#include <liara/core/core_export.h>
#include <liara/internal/portability.h>

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
 * This is the core's half of the render packet pattern described in ARCHITECTURE.md \S6. It is the host's
 * responsibility to call this after each `liara_core_update` tick (typically from a late update callback, see
 * `liara_core_set_late_update_callback`) and to hand the resulting packet to a renderer via
 * `liara_renderer_submit_frame`. The core never calls the renderer itself (MODULES.md \S10): the data path is
 * always core -> host -> renderer.
 *
 * @param[in] core_handle A pointer to the core instance whose render packet is to be retrieved.
 * @param[out] out_packet A pointer to a `liara_render_packet_t` that will receive a copy of the packet.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The render packet was retrieved successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `core_handle` or `out_packet` parameter is a null pointer.
 * - `LIARA_RESULT_INVALID_STATE`: The core instance is in an invalid state (e.g., already destroyed).
 *
 * @warning The `out_packet->drawables` pointer refers to memory owned by the core and is only valid until the next
 *          call to `liara_core_update` or `liara_core_run` (or until `core_handle` is destroyed). The host must
 *          finish consuming the packet (typically by passing it to `liara_renderer_submit_frame`) before triggering
 *          the next tick; it must not retain the pointer across ticks.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that
 * modify the core's state. @endthreadsafety
 */
LIARA_CORE_API liara_result_t liara_core_get_render_packet(const liara_core_handle_t* core_handle,
                                                           liara_render_packet_t* out_packet);

#ifdef __cplusplus
}
#endif
