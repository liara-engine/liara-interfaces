---
title: Error handling
description: One result type, output parameters with a mandatory prefix, and the absolute rule about what may not unwind through the boundary.
sidebar:
  order: 3
---

## The result type

A function that can fail returns a `liara_result_t`, an enum whose constants are prefixed and grouped by category.

```c
typedef enum {
    LIARA_RESULT_SUCCESS = 0,
    LIARA_RESULT_<ERROR_NAME_N> = -<N>,
    // ...
} liara_result_t;
```

The codes are declared through X-macros, so that the enum, the name table and anything else derived from them stay in step by construction rather than by discipline. They are sorted by category, with -2 to -99 reserved for generic failures such as out of memory and invalid argument.

`LIARA_RESULT_SUCCESS` is always zero, so `if (result)` reads as "if something went wrong", which is what a C programmer expects.

A function that cannot fail returns `void`. Returning `liara_result_t` unconditionally is noise, and a signature should say honestly whether the call can fail.

## Output parameters

A function producing a value passes it back through an output parameter rather than a return value, since the return value is already carrying the result code.

```c
liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,
    liara_renderer_handle_t** out_handle
);
```

The `out_` prefix is mandatory, and the interface-rules lint in CI checks it rather than leaving it to review.

An output parameter may not be null. Calling with `out_handle == NULL` is undefined, and a function assumes its output parameters are valid pointers.

## Nothing unwinds through the boundary

An implementation may use exceptions, panics or `longjmp` internally, and none of them may cross.

A C++ implementation catching its exceptions at the boundary and translating them into a `liara_result_t` is conforming. One that lets an exception propagate out of an interface function is not, and the behavior is undefined rather than merely wrong: the C caller has no way to catch it, and unwinding through a foreign frame is undefined by the standard.

The renderer carries the sharpest version of this. Vulkan errors and GPU failures are handled inside the module and reported as result codes, or, when they arrive asynchronously, through a [callback](./callbacks/).
