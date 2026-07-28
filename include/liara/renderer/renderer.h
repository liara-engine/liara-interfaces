#pragma once

#include <liara/private_utils.h>
#include <liara/renderer/packet.h>
#include <liara/renderer/renderer_export.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

LIARA_TYPEDEF(struct liara_module_info_t, liara_module_info_t);
LIARA_TYPEDEF(struct liara_core_t, liara_core_handle_t);

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
 * @brief Opaque structure representing a Liara renderer instance.
 *
 * This structure is used to encapsulate the internal state and resources associated with a Liara renderer. The actual
 * implementation details are hidden from the user, and the structure should only be manipulated through the provided
 * API functions.
 */
LIARA_TYPEDEF(struct liara_renderer_t, liara_renderer_handle_t);

/**
 * @brief Returns information about the Liara renderer module.
 *
 * This function returns a pointer to a `liara_module_info_t` structure containing information about the Liara renderer
 * module, including its version, ABI version, and name. The returned pointer is valid for the lifetime of the program
 * and should not be modified or freed by the caller.
 *
 * @return A pointer to a `liara_module_info_t` structure containing information about the Liara renderer module.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_RENDERER_API const liara_module_info_t* liara_renderer_info(void);

/**
 * @brief Returns the current version of the Liara renderer ABI interface.
 *
 * This function returns the current version of the Liara renderer ABI interface as a 32-bit unsigned integer. The
 * version is encoded using the same scheme as the Liara ABI version, with major, minor, and patch components.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara renderer ABI interface.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_RENDERER_API uint32_t liara_renderer_abi_version(void);

// TODO: Remove this function (marked as deprecated in v0.1.1)
/**
 * @deprecated This function is deprecated and will be removed in future versions. Use `liara_renderer_info` instead.
 * @brief Returns the current version of the Liara renderer.
 *
 * This function returns the current version of the Liara renderer as a 32-bit unsigned integer. The version is encoded
 * using the same scheme as the Liara ABI version, with major, minor, and patch components.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara renderer.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and will be removed in future versions. Use liara_renderer_info instead.")
LIARA_RENDERER_API uint32_t liara_renderer_version(void);

// TODO: Update this function to use `liara_result_t` instead of `liara_result` in v0.2.0
/**
 * @deprecated This function is deprecated and its signature will use `liara_result_t` instead of `liara_result` in
 * v0.2.0. Please update your code accordingly.
 * @brief Creates a new Liara renderer instance.
 *
 * This function allocates and initializes a new Liara renderer instance. The created renderer is returned through the
 * `out_renderer` parameter. The caller is responsible for destroying the renderer using `liara_renderer_destroy`.
 *
 * @param[out] out_renderer A pointer to a pointer that will receive the newly created renderer instance.
 *
 * @return A `liara_result` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The renderer was created successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `out_renderer` parameter is a null pointer.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with `liara_renderer_destroy` on
 * the same renderer instance. @endthreadsafety
 */
LIARA_API_DEPRECATED("This function is deprecated and its signature will use `liara_result_t` instead of "
                     "`liara_result` in v0.2.0. Please update your code accordingly.")
LIARA_RENDERER_API liara_result liara_renderer_create(liara_renderer_handle_t** out_renderer);

// TODO: Update this function to use `liara_result_t` instead of `liara_result` in v0.2.0
/**
 * @deprecated This function is deprecated and its signature will use `liara_result_t` instead of `liara_result` in
 * v0.2.0. Please update your code accordingly.
 * @brief Destroys a Liara renderer instance.
 *
 * This function deallocates and cleans up the resources associated with a Liara renderer instance. After calling this
 * function, the renderer pointer should not be used again.
 *
 * @param[in] renderer A pointer to the renderer instance to be destroyed.
 *
 * @return A `liara_result` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The renderer was destroyed successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `renderer` parameter is a null pointer.
 * - `LIARA_RESULT_INVALID_STATE`: The renderer instance is in an invalid state (e.g., already destroyed).
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with `liara_renderer_create` on
 * the same renderer instance. @endthreadsafety
 */
LIARA_API_DEPRECATED("This function is deprecated and its signature will use `liara_result_t` instead of "
                     "`liara_result` in v0.2.0. Please update your code accordingly.")
LIARA_RENDERER_API liara_result liara_renderer_destroy(const liara_renderer_handle_t* renderer);

// TODO: Remove this function (marked as deprecated in v0.1.1)
/**
 * @deprecated This function is deprecated and no longer has any effect. Use `liara_renderer_submit_frame` instead.
 * @brief Prints a message using the Liara renderer.
 *
 * This function sends a message to be printed by the Liara renderer. The message is provided as a string and its length
 * is specified by the `message_length` parameter.
 *
 * @param[in] renderer A pointer to the renderer instance used for printing the message.
 * @param[in] message A pointer to the message string to be printed.
 * @param[in] message_length The length of the message string in bytes.
 *
 * @return A `liara_result` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The arguments were valid (the message is not actually printed anymore).
 * - `LIARA_RESULT_NULL_POINTER`: The `renderer` or `message` parameter is a null pointer.
 * - `LIARA_RESULT_INVALID_ARGUMENT`: The `message_length` parameter is zero.
 * - `LIARA_RESULT_INVALID_STATE`: The renderer instance is in an invalid state (e.g., not initialized).
 *
 * @note This was a demonstration-only function. It is now a validating no-op, kept only so existing callers don't
 *       break; it will be removed in a future major version. Use `liara_renderer_submit_frame` to actually draw.
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and no longer has any effect. Use liara_renderer_submit_frame instead.")
LIARA_RENDERER_API liara_result liara_renderer_print(const liara_renderer_handle_t* renderer,
                                                     const char* message,
                                                     size_t message_length);

// TODO: Remove this function (marked as deprecated in v0.1.1)
/**
 * @deprecated This function is deprecated and no longer has any effect. Use `liara_renderer_submit_frame` instead.
 * @brief Prints a message with a newline using the Liara renderer.
 *
 * This function sends a message to be printed by the Liara renderer, followed by a newline character. The message is
 * provided as a string and its length is specified by the `message_length` parameter.
 *
 * @param[in] renderer A pointer to the renderer instance used for printing the message.
 * @param[in] message A pointer to the message string to be printed.
 * @param[in] message_length The length of the message string in bytes.
 *
 * @return A `liara_result` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The arguments were valid (the message is not actually printed anymore).
 * - `LIARA_RESULT_NULL_POINTER`: The `renderer` or `message` parameter is a null pointer.
 * - `LIARA_RESULT_INVALID_ARGUMENT`: The `message_length` parameter is zero.
 * - `LIARA_RESULT_INVALID_STATE`: The renderer instance is in an invalid state (e.g., not initialized).
 *
 * @note This was a demonstration-only function. It is now a validating no-op, kept only so existing callers don't
 *       break; it will be removed in a future major version. Use `liara_renderer_submit_frame` to actually draw.
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and no longer has any effect. Use liara_renderer_submit_frame instead.")
LIARA_RENDERER_API liara_result liara_renderer_println(const liara_renderer_handle_t* renderer,
                                                       const char* message,
                                                       size_t message_length);

// TODO: Remove this function (marked as deprecated in v0.1.1)
/**
 * @deprecated This function is deprecated and no longer has any effect. Use `liara_renderer_submit_frame` instead.
 * @brief Sets the text color for the Liara renderer.
 *
 * This function sets the text color used by the Liara renderer for subsequent print operations. The color is specified
 * as a 32-bit unsigned integer, typically in ARGB format.
 *
 * @param[in] renderer A pointer to the renderer instance for which to set the text color.
 * @param[in] color A 32-bit unsigned integer representing the desired text color (e.g., ARGB format).
 *
 * @note This was a demonstration-only function. It is now a no-op, kept only so existing callers don't break; it
 *       will be removed in a future major version. Color is now conveyed per-drawable through render packets
 *       submitted via `liara_renderer_submit_frame`.
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and no longer has any effect. Use liara_renderer_submit_frame instead.")
LIARA_RENDERER_API void liara_renderer_set_text_color(const liara_renderer_handle_t* renderer, uint32_t color);

// TODO: Remove this function (marked as deprecated in v0.1.1)
/**
 * @deprecated This function is deprecated and no longer has any effect. Use `liara_renderer_submit_frame` instead.
 * @brief Sets the background color for the Liara renderer.
 *
 * This function sets the background color used by the Liara renderer for subsequent print operations. The color is
 * specified as a 32-bit unsigned integer, typically in ARGB format.
 *
 * @param[in] renderer A pointer to the renderer instance for which to set the background color.
 * @param[in] color A 32-bit unsigned integer representing the desired background color (e.g., ARGB format).
 *
 * @note This was a demonstration-only function. It is now a no-op, kept only so existing callers don't break; it
 *       will be removed in a future major version. Background color is now conveyed through
 *       `liara_render_packet_t::background_color`, submitted via `liara_renderer_submit_frame`.
 */
LIARA_API_DEPRECATED(
    "This function is deprecated and no longer has any effect. Use liara_renderer_submit_frame instead.")
LIARA_RENDERER_API void liara_renderer_set_background_color(const liara_renderer_handle_t* renderer, uint32_t color);

/**
 * @brief Submits one frame's render packet to the renderer for presentation.
 *
 * This is the entry point for the render packet pattern described in ARCHITECTURE.md \S6: the host extracts a
 * packet from the core (via `liara_core_get_render_packet`) and hands it to the renderer here. The core never calls
 * this function itself (see MODULES.md \S10); composition is always the host's responsibility.
 *
 * @param[in] renderer A pointer to the renderer instance that will present the packet.
 * @param[in] packet A pointer to the render packet to present.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The frame was presented successfully.
 * - `LIARA_RESULT_NULL_POINTER`: The `renderer` or `packet` parameter is a null pointer, or `packet->drawables` is
 *   null while `packet->drawable_count` is non-zero.
 * - `LIARA_RESULT_INVALID_ARGUMENT`: `packet->grid_width` or `packet->grid_height` is zero.
 * - `LIARA_RESULT_INVALID_STATE`: The renderer instance is in an invalid state (e.g., not initialized).
 *
 * @note The renderer does not retain ownership of `packet` or `packet->drawables` beyond the call; it may present
 *       the data however it sees fit (in Phase 0, as a grid of colored terminal cells) and does not need to be
 *       informed when the packet's backing memory is reused or freed by the caller.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with other functions that
 * modify the renderer's state. @endthreadsafety
 */
LIARA_RENDERER_API liara_result_t liara_renderer_submit_frame(liara_renderer_handle_t* renderer,
                                                              const liara_render_packet_t* packet);

#ifdef __cplusplus
}
#endif
