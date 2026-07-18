#pragma once

#include <liara/renderer/renderer.h>
#include <liara/result.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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
uint32_t liara_core_abi_version(void);

/**
 * @brief Returns the current version of the Liara core.
 *
 * This function returns the current version of the Liara core as a 32-bit unsigned integer. The version is encoded
 * using the same scheme as the Liara ABI version, with major, minor, and patch components.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara core.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
uint32_t liara_core_version(void);

/**
 * @brief Opaque structure representing a Liara core instance.
 *
 * This structure is used to encapsulate the internal state and resources associated with a Liara core. The actual
 * implementation details are hidden from the user, and the structure should only be manipulated through the provided
 * API functions.
 */
typedef struct liara_core_t liara_core_t;

/**
 * @brief Creates a new Liara core instance.
 *
 * This function allocates and initializes a new Liara core instance. The created core is returned through the
 * `out_core` parameter. The caller is responsible for destroying the core using `liara_core_destroy`.
 *
 * @param[in] renderer A pointer to the renderer instance to be used by the core.
 * @param[out] out_core A pointer to a pointer that will receive the newly created core instance.
 *
 * @return A `liara_result` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The core was created successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `out_core` or `renderer` parameter is a null pointer.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with `liara_core_destroy` on
 * the same core instance. @endthreadsafety
 */
liara_result liara_core_create(liara_renderer_t* renderer, liara_core_t** out_core);

/**
 * @brief Destroys a Liara core instance.
 *
 * This function deallocates and cleans up the resources associated with a Liara core instance. After calling this
 * function, the core pointer should not be used again.
 *
 * @param[in] core A pointer to the core instance to be destroyed.
 *
 * @return A `liara_result` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The core was destroyed successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `core` parameter is a null pointer.
 */
liara_result liara_core_destroy(const liara_core_t* core);

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
 * @param[in] core A pointer to the core instance whose run mode is to be set.
 * @param[in] run_mode The desired run mode for the core.
 * @param[in] fixed_time_step The fixed time step to use when in fixed run mode. Ignored for other modes.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
void liara_core_set_run_mode(liara_core_t* core, enum liara_core_run_mode run_mode, float fixed_time_step);

/**
 * @brief Runs the Liara core's update loop.
 *
 * This function starts the Liara core's update loop in automatic or fixed run mode. It will continuously update and
 * render until the core is stopped or destroyed. In manual run mode, this function has no effect.
 *
 * @param[in] core A pointer to the core instance to run.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
void liara_core_run(liara_core_t* core);

/**
 * @brief Stops the Liara core's update loop.
 *
 * This function stops the Liara core's update loop if it is currently running in automatic or fixed run mode. In manual
 * run mode, this function has no effect.
 *
 * @param[in] core A pointer to the core instance to stop.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
void liara_core_stop(liara_core_t* core);

/**
 * @brief Updates the Liara core in manual run mode.
 *
 * This function performs an update of the Liara core when it is in manual run mode. The `delta_time` parameter
 * specifies the time elapsed since the last update, allowing for time-based calculations within the core.
 * In automatic or fixed run modes, this function has no effect, as updates are handled automatically.
 *
 * @param[in] core A pointer to the core instance to update.
 * @param[in] delta_time The time elapsed since the last update, in seconds.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
void liara_core_update(liara_core_t* core, float delta_time);

/**
 * @brief Sets the late update callback for the Liara core.
 *
 * This function sets a callback function that will be called during the late update phase of the Liara core's update
 * loop. The callback receives the core instance and the delta time since the last update.
 *
 * @param[in] core A pointer to the core instance for which to set the late update callback.
 * @param[in] callback A pointer to the callback function to be called during late updates. If `NULL`, no callback will
 * be called.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that modify
 * the core's state. @endthreadsafety
 */
void liara_core_set_late_update_callback(liara_core_t* core, void (*callback)(liara_core_t* core, float delta_time));

#ifdef __cplusplus
}
#endif
