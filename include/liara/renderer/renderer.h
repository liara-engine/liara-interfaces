#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Returns the current version of the Liara renderer ABI interface.
 *
 * This function returns the current version of the Liara renderer ABI interface as a 32-bit unsigned integer. The version
 * is encoded using the same scheme as the Liara ABI version, with major, minor, and patch components.
 *
 * @return A 32-bit unsigned integer representing the current version of the Liara renderer ABI interface.
 *
 * @threadsafety This function is thread-safe as it does not modify any shared state. @endthreadsafety
 */
uint32_t liara_renderer_abi_version(void);

/**
 * @brief Opaque structure representing a Liara renderer instance.
 *
 * This structure is used to encapsulate the internal state and resources associated with a Liara renderer. The actual
 * implementation details are hidden from the user, and the structure should only be manipulated through the provided
 * API functions.
 */
typedef struct liara_renderer_t liara_renderer_t;

/**
 * @brief Creates a new Liara renderer instance.
 *
 * This function allocates and initializes a new Liara renderer instance. The created renderer is returned through the
 * `out_renderer` parameter. The caller is responsible for destroying the renderer using `liara_renderer_destroy`.
 *
 * @param[out] out_renderer A pointer to a pointer that will receive the newly created renderer instance.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with `liara_renderer_destroy` on
 * the same renderer instance. @endthreadsafety
 */
void liara_renderer_create(liara_renderer_t** out_renderer);

/**
 * @brief Destroys a Liara renderer instance.
 *
 * This function deallocates and cleans up the resources associated with a Liara renderer instance. After calling this
 * function, the renderer pointer should not be used again.
 *
 * @param[in] renderer A pointer to the renderer instance to be destroyed.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with `liara_renderer_create` on
 * the same renderer instance. @endthreadsafety
 */
void liara_renderer_destroy(const liara_renderer_t* renderer);

/**
 * @brief Prints a message using the Liara renderer.
 *
 * This function sends a message to be printed by the Liara renderer. The message is provided as a string and its length
 * is specified by the `message_length` parameter.
 *
 * @param[in] renderer A pointer to the renderer instance used for printing the message.
 * @param[in] message A pointer to the message string to be printed.
 * @param[in] message_length The length of the message string in bytes.
 *
 * @note This function is just for demonstration purposes and can be removed at any time, without notice.
 */
void liara_renderer_print(const liara_renderer_t* renderer, const char* message, size_t message_length);

/**
 * @brief Sets the text color for the Liara renderer.
 *
 * This function sets the text color used by the Liara renderer for subsequent print operations. The color is specified
 * as a 32-bit unsigned integer, typically in ARGB format.
 *
 * @param[in] renderer A pointer to the renderer instance for which to set the text color.
 * @param[in] color A 32-bit unsigned integer representing the desired text color (e.g., ARGB format).
 *
 * @note This function is just for demonstration purposes and can be removed at any time, without notice.
 */
void liara_renderer_set_text_color(const liara_renderer_t* renderer, uint32_t color);

/**
 * @brief Sets the background color for the Liara renderer.
 *
 * This function sets the background color used by the Liara renderer for subsequent print operations. The color is specified
 * as a 32-bit unsigned integer, typically in ARGB format.
 *
 * @param[in] renderer A pointer to the renderer instance for which to set the background color.
 * @param[in] color A 32-bit unsigned integer representing the desired background color (e.g., ARGB format).
 *
 * @note This function is just for demonstration purposes and can be removed at any time, without notice.
 */
void liara_renderer_set_background_color(const liara_renderer_t* renderer, uint32_t color);

#ifdef __cplusplus
}
#endif
