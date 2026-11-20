#pragma once

#include <liara/extent.h>
#include <liara/internal/portability.h>
#include <liara/native_window.h>
#include <liara/renderer/renderer_export.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

LIARA_TYPEDEF(struct liara_module_info_t, liara_module_info_t);
LIARA_TYPEDEF(struct liara_render_packet_t, liara_render_packet_t);
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
 * @brief Everything `liara_renderer_create` needs in order to create a renderer.
 */
LIARA_TYPEDEF_STRUCT(
    liara_renderer_create_info_t, uint32_t struct_version; /**< LIARA_RENDERER_CREATE_INFO_VERSION. */
    uint32_t reserved;                                     /**< Explicit padding. Set to zero; version 1 ignores it. */
    /**
     * The window the renderer presents to, as `liara_platform_window_get_native` describes it.
     * `LIARA_NATIVE_WINDOW_KIND_NONE` asks for a renderer without a surface, which creates its GPU device and presents
     * nothing. The pointers it holds are borrowed for the renderer's whole life, not only for the call: the window
     * outlives the renderer.
     */
    liara_native_window_t native_window;
    /**
     * The window's current size, in pixels, as `liara_platform_window_get_pixel_size` reports it. Some window systems
     * give a surface no size of its own, and the renderer then sizes its images from this. Ignored without a window.
     */
    liara_extent2d_t pixel_size;
    /**
     * A preference among the GPUs, as UTF-8, borrowed for the call. Among the devices the renderer can use, those whose
     * name contains this text, compared case-insensitively over ASCII, are preferred. When none of them can be used,
     * the renderer chooses as it would without a preference and reports why through its diagnostics. Null only when
     * `preferred_device_name_length` is 0.
     */
    const char* preferred_device_name;
    size_t preferred_device_name_length; /**< In bytes, no terminator required. 0: no preference. */
);

#define LIARA_RENDERER_CREATE_INFO_VERSION 1U /**< Current version of liara_renderer_create_info_t. */

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

/**
 * @brief Creates a new Liara renderer instance.
 *
 * This function allocates and initializes a new Liara renderer instance: it loads the Vulkan implementation and creates
 * the GPU device the renderer draws with. Given a window, it also creates a surface on that window and a swapchain to
 * present through. Given `LIARA_NATIVE_WINDOW_KIND_NONE`, it creates no surface, and `liara_renderer_submit_frame`
 * presents nothing. The caller is responsible for destroying the renderer using `liara_renderer_destroy`, before the
 * window it presents to is destroyed.
 *
 * @param[in] create_info Describes the renderer. It may not be null. It is read during the call only, except for the
 *            pointers inside `native_window`, which the renderer keeps until it is destroyed.
 * @param[out] out_renderer A pointer to a pointer that will receive the newly created renderer instance. It is left
 *             untouched unless the call succeeds.
 *
 * @return A `liara_result_t` indicating the success or failure of the operation. Possible return values include:
 * - `LIARA_RESULT_SUCCESS`: The renderer was created successfully.
 * - `LIARA_RESULT_NULL_POINTER`: `create_info` or `out_renderer` is null, `preferred_device_name` is null while its
 *   length is not zero, or a pointer the window's kind needs is null.
 * - `LIARA_RESULT_VERSION_MISMATCH`: `create_info->struct_version` is not a version this module understands.
 * - `LIARA_RESULT_NOT_SUPPORTED`: The window's kind is one this build cannot present to; no usable Vulkan 1.3
 *   implementation was found (no loader, no driver, or no suitable device, including one that can present to the
 *   window); or a component that a debug build of the module requires is missing.
 * - `LIARA_RESULT_INVALID_ARGUMENT`: A window is given and a component of `pixel_size` is zero.
 * - `LIARA_RESULT_OUT_OF_MEMORY`: Host or device memory ran out.
 * - `LIARA_RESULT_FAILED`: Any other failure of the graphics API, including creating the surface or the swapchain, or a
 *   window with no area to present to.
 *
 * @threadsafety This function is thread-safe as long as it is not called concurrently with `liara_renderer_destroy` on
 * the same renderer instance. What the window system allows for the window applies as well. @endthreadsafety
 */
LIARA_RENDERER_API liara_result_t liara_renderer_create(const liara_renderer_create_info_t* create_info,
                                                        liara_renderer_handle_t** out_renderer);

/**
 * @brief Destroys a Liara renderer instance.
 *
 * This function deallocates and cleans up the resources associated with a Liara renderer instance. After calling this
 * function, the renderer pointer must not be used again.
 *
 * @param[in] renderer A pointer to the renderer instance to be destroyed. A null pointer is a no-op, as it is for
 *            `free`. Passing a handle that was already destroyed is undefined.
 *
 * @threadsafety This function is not thread-safe with respect to the handle it destroys. @endthreadsafety
 */
LIARA_RENDERER_API void liara_renderer_destroy(liara_renderer_handle_t* renderer);

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
 * - `LIARA_RESULT_SURFACE_OUT_OF_DATE`: The swapchain no longer matches the window, which was resized or minimised.
 *   Nothing was presented.
 * - `LIARA_RESULT_SURFACE_LOST`: The window system took the surface away. Nothing was presented.
 * - `LIARA_RESULT_OUT_OF_MEMORY`: Memory ran out while drawing the frame.
 * - `LIARA_RESULT_FAILED`: Any other failure while drawing the frame.
 *
 * When it returns `LIARA_RESULT_SURFACE_OUT_OF_DATE` or `LIARA_RESULT_SURFACE_LOST`, the console has still drawn the
 * packet, and every later call returns the same code until the renderer can recreate its swapchain. A renderer created
 * without a window never returns either.
 *
 * @note The renderer does not retain ownership of `packet` or `packet->drawables` beyond the call; it may present
 *       the data however it sees fit (for now, as a grid of colored terminal cells) and does not need to be
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
