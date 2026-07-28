#pragma once

#include <liara/core/core_export.h>
#include <liara/private_utils.h>
#include <liara/renderer/packet.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

LIARA_TYPEDEF(struct liara_module_info_t, liara_module_info_t);
LIARA_TYPEDEF(struct liara_renderer_t, liara_renderer_handle_t);

// TODO: Remove (marked as deprecated in v0.1.1)
/**
 * @deprecated: This type is deprecated and will be removed in future versions. Use `liara_result_t` instead. \
 */
LIARA_TYPEDEF_DEPRECATED(
    int32_t,
    liara_result,
    "This type is deprecated and will be removed in future versions. Use `liara_result_t` instead.");
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

// TODO: Remove this function (marked as deprecated in v0.1.1)
/**
 * @deprecated This function is deprecated and will be removed in future versions. Use `liara_core_info` instead.
 * @brief Returns the current version of the Liara core.
 *
 * This function returns the current version of the Liara core as a 32-bit unsigned integer. The version is encoded
 * using the same scheme as the Liara ABI version, with major, minor, and patch components.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara core.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED("This function is deprecated and will be removed in future versions. Use liara_core_info instead.")
LIARA_CORE_API uint32_t liara_core_version(void);

// TODO: Update this function to use `liara_result_t` instead of `liara_result` in v0.2.0
/**
 * @deprecated This function is deprecated and its signature will use `liara_result_t` instead of `liara_result` in
 * v0.2.0. Please update your code accordingly.
 * @brief Creates a new Liara core instance.
 *
 * This function allocates and initializes a new Liara core instance. The created core is returned through the
 * `out_core` parameter. The caller is responsible for destroying the core using `liara_core_destroy`.
 *
 * @param[in] renderer_handle A pointer to the renderer instance to be used by the core.
 * @param[out] out_core A pointer to a pointer that will receive the newly created core instance.
 *
 * @return A `liara_result` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The core was created successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `out_core` or `renderer_handle` parameter is a null pointer.
 *
 * @note The core does not call into `renderer_handle` itself; per ARCHITECTURE.md \S5 and MODULES.md \S10, the
 *       core never directly calls the renderer. This parameter can't be dropped without changing this already-frozen
 *       signature, so it is accepted and validated but otherwise unused by the core. To actually get pixels on
 *       screen, the host extracts a render packet each tick via `liara_core_get_render_packet` and hands it to the
 *       renderer via `liara_renderer_submit_frame`.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with `liara_core_destroy` on
 * the same core instance. @endthreadsafety
 */
LIARA_API_DEPRECATED("This function is deprecated and its signature will use `liara_result_t` instead of "
                     "`liara_result` in v0.2.0. Please update your code accordingly.")
LIARA_CORE_API liara_result liara_core_create(liara_renderer_handle_t* renderer_handle, liara_core_handle_t** out_core);

// TODO: Update this function to use `liara_result_t` instead of `liara_result` in v0.2.0
/**
 * @deprecated This function is deprecated and its signature will use `liara_result_t` instead of `liara_result` in
 * v0.2.0. Please update your code accordingly.
 * @brief Destroys a Liara core instance.
 *
 * This function deallocates and cleans up the resources associated with a Liara core instance. After calling this
 * function, the core pointer should not be used again.
 *
 * @param[in] core_handle A pointer to the core instance to be destroyed.
 *
 * @return A `liara_result` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The core was destroyed successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `core` parameter is a null pointer.
 */
LIARA_API_DEPRECATED("This function is deprecated and its signature will use `liara_result_t` instead of "
                     "`liara_result` in v0.2.0. Please update your code accordingly.")
LIARA_CORE_API liara_result liara_core_destroy(const liara_core_handle_t* core_handle);

/**
 * @brief Enumeration representing the run modes of the Liara core.
 *
 * This enumeration defines the different run modes that the Liara core can operate in. Each mode determines how the
 * core handles updates and rendering, allowing for flexibility in application behavior.
 */
enum liara_core_run_mode {
    /**
     * @brief Manual run mode.
     *
     * In this mode, the Liara core will not automatically update or render. The user must manually trigger updates and
     * rendering, specifying the delta time for each update.
     * Useful for editors or applications that require precise control over the update loop.
     */
    LIARA_CORE_RUN_MODE_MANUAL = 0,
    /**
     * @brief Automatic run mode.
     *
     * In this mode, the Liara core will automatically as fast as possible update and render. The user does not need to
     * manually trigger updates or rendering.
     * Default mode, suitable for most games or applications.
     */
    LIARA_CORE_RUN_MODE_AUTOMATIC = 1,
    /**
     * @brief Automatic run mode.
     *
     * In this mode, the Liara core will automatically update and render at a fixed time step. The user does not need to
     * manually trigger updates or rendering.
     * Useful for games or applications that require a consistent update loop without manual intervention.
     */
    LIARA_CORE_RUN_MODE_FIXED = 2,
};

/**
 * @brief Sets the run mode of the Liara core.
 *
 * This function sets the run mode of the Liara core, determining how it handles updates and rendering. The `run_mode`
 * parameter specifies the desired run mode, and the `fixed_time_step` parameter is used when the fixed run mode is
 * selected.
 *
 * @param[in] core_handle A pointer to the core instance whose run mode is to be set.
 * @param[in] run_mode The desired run mode for the core.
 * @param[in] fixed_time_step The fixed time step to use when in fixed run mode. Ignored for other modes.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
LIARA_CORE_API void liara_core_set_run_mode(liara_core_handle_t* core_handle,
                                            enum liara_core_run_mode run_mode,
                                            float fixed_time_step);

/**
 * @brief Runs the Liara core's update loop.
 *
 * This function starts the Liara core's update loop in automatic or fixed run mode. It will continuously update and
 * render until the core is stopped or destroyed. In manual run mode, this function has no effect.
 *
 * @param[in] core_handle A pointer to the core instance to run.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
LIARA_CORE_API void liara_core_run(liara_core_handle_t* core_handle);

/**
 * @brief Stops the Liara core's update loop.
 *
 * This function stops the Liara core's update loop if it is currently running in automatic or fixed run mode. In manual
 * run mode, this function has no effect.
 *
 * @param[in] core_handle A pointer to the core instance to stop.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
LIARA_CORE_API void liara_core_stop(liara_core_handle_t* core_handle);

/**
 * @brief Updates the Liara core in manual run mode.
 *
 * This function performs an update of the Liara core when it is in manual run mode. The `delta_time` parameter
 * specifies the time elapsed since the last update, allowing for time-based calculations within the core.
 * In automatic or fixed run modes, this function has no effect, as updates are handled automatically.
 *
 * @param[in] core_handle A pointer to the core instance to update.
 * @param[in] delta_time The time elapsed since the last update, in seconds.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
LIARA_CORE_API void liara_core_update(liara_core_handle_t* core_handle, float delta_time);

/**
 * @brief Sets the late update callback for the Liara core.
 *
 * This function sets a callback function that will be called during the late update phase of the Liara core's update
 * loop. The callback receives the core instance and the delta time since the last update.
 *
 * @param[in] core_handle A pointer to the core instance for which to set the late update callback.
 * @param[in] callback A pointer to the callback function to be called during late updates. If `NULL`, no callback will
 * be called.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
LIARA_CORE_API void liara_core_set_late_update_callback(liara_core_handle_t* core_handle,
                                                        void (*callback)(liara_core_handle_t* core, float delta_time));

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
