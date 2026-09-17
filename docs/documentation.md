---
title: Documentation
description: What every public symbol carries, and why the header rather than the generated page is the canonical reference.
sidebar:
  order: 8
---

Every public symbol carries Doxygen documentation, and it documents what a consumer cannot see from the signature: which pointers may be null, who owns what, what the valid lifetime of a returned pointer is, which result codes can come back, and whether the call is thread-safe.

```c
/**
 * @param[in] create_info  Configuration for the renderer. Must not be NULL.
 *                         Read during the call only; not retained.
 * @param[in] allocator    Optional custom allocator. May be NULL to use
 *                         the module's default allocator. If non-NULL, the
 *                         pointed-to allocator must remain valid for the
 *                         lifetime of the returned handle.
 * @param[out] out_handle  Output: the created renderer handle. Must not be
 *                         NULL. On success, *out_handle is set to a valid
 *                         handle that the caller must eventually pass to
 *                         liara_renderer_destroy. On failure, *out_handle
 *                         is set to NULL.
 * @return LIARA_RESULT_SUCCESS, LIARA_RESULT_OUT_OF_MEMORY, or
 *         LIARA_RESULT_INVALID_ARGUMENT.
 *
 * @threadsafety This function is not thread-safe; only one thread
 *               may call it at a time. @endthreadsafety
 */
liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,
    const liara_allocator_t* allocator,
    liara_renderer_handle_t** out_handle
);
```

The header is the primary documentation. Doxygen runs on it to produce the browsable API reference, through XML that the documentation pipeline turns into pages, and the header's source stays the canonical version. Someone reading it in an editor, with no site open, should get everything the page would have given them.
