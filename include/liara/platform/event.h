/**
 * @file event.h
 * @brief The events a platform instance reports, and the call that returns them one at a time.
 */

#pragma once

#include <liara/extent.h>
#include <liara/internal/portability.h>
#include <liara/platform/platform.h>
#include <liara/platform/platform_export.h>
#include <liara/platform/window.h>

#ifndef __cplusplus
    #include <stdbool.h>
#endif
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Which member of `liara_platform_event_t.data` is valid. One of the `LIARA_PLATFORM_EVENT_*` values.
 */
LIARA_TYPEDEF(uint32_t, liara_platform_event_type_t);

/**
 * @brief The values of `liara_platform_event_type_t`.
 *
 * New types are appended in minor versions, and a module newer than its host returns them. A host skips any type it
 * does not recognise.
 */
enum liara_platform_event_types {
    LIARA_PLATFORM_EVENT_NONE = 0,                   /**< Never returned; the zero value. */
    LIARA_PLATFORM_EVENT_WINDOW_CLOSE_REQUESTED = 1, /**< The user asked `window` to close. It is not destroyed. */
    LIARA_PLATFORM_EVENT_WINDOW_RESIZED = 2,         /**< `window` changed size; `data.resized` holds the new one. */
};

/**
 * @brief One event, in a record of fixed size.
 *
 * The caller allocates it, so its size is part of the contract: 64 bytes on 64-bit targets. (32-bit x86 Linux, whose
 * ABI aligns a `uint64_t` to 4 bytes inside a struct, lays it out in 60.) The payload of a type added later fits inside
 * `data.padding`.
 *
 * @threadsafety Plain data, with no thread-safety concerns of its own. @endthreadsafety
 */
LIARA_TYPEDEF_STRUCT(
    liara_platform_event_t, liara_platform_event_type_t type; /**< Which member of `data` is valid. */
    uint32_t reserved;                                        /**< Explicit padding. Zero. */
    uint64_t timestamp_ns;                   /**< liara_platform_time_now_ns when the module translated the event. */
    liara_platform_window_handle_t * window; /**< The window concerned, live when returned; or null. */
    union {
        struct {
            liara_extent2d_t pixel_size; /**< The new size, in pixels. */
        } resized;                       /**< Valid for `LIARA_PLATFORM_EVENT_WINDOW_RESIZED`. */
        uint64_t padding[5];             /**< Fixes the size of the union. Never read. */
    } data;                              /**< The payload, per `type`. */
);

/**
 * @brief Returns the next pending event of an instance, one per call.
 *
 * A drain is the run of calls up to one that returns `false`. Its first call pumps the operating system once; the
 * others only read what is already queued for this instance, which a poll on another instance may add to. A host
 * looping `while (liara_platform_poll_event(...))` over one instance therefore terminates, even under a continuous
 * stream of events such as a resize drag. An instance sees the events of its own windows only.
 *
 * Window close requests reach `liara_platform_quit_requested` through this call and no other, so a host polls its
 * events before it reads the quit flag.
 *
 * @param[in] platform The instance to poll. Passing null is a programming error.
 * @param[out] out_event Receives the event. Untouched when the call returns `false`.
 *
 * @return `true` when an event was written to `out_event`, `false` when the drain is over.
 *
 * @threadsafety Call from the process's main thread only. @endthreadsafety
 */
LIARA_PLATFORM_API bool liara_platform_poll_event(liara_platform_handle_t* platform, liara_platform_event_t* out_event);

#ifdef __cplusplus
}
#endif
