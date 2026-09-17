---
title: Types
description: Fixed-width integers, opaque handles, plain-data structs, and the two ways a string may cross.
sidebar:
  order: 2
---

## Fixed-width integers only

Where size matters, and at a boundary it always does, only fixed-width types appear. Plain `int`, `long` and `short` are forbidden in an interface signature, because their size varies across platforms.

```c
// Good
uint32_t width;
int64_t timestamp_ns;

// Bad
unsigned width;
long timestamp;
```

`bool` is the exception and is allowed, with an implementation-defined size. Anything needing a specific size for a boolean uses `uint8_t`.

## Opaque handles

An object owned by a module is exposed to other modules as an opaque handle: a pointer to a struct that is forward-declared publicly and defined privately.

```c title="liara/renderer/renderer.h"
LIARA_TYPEDEF(struct liara_renderer_t, liara_renderer_handle_t);

liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,
    liara_renderer_handle_t** out_handle
);

void liara_renderer_destroy(liara_renderer_handle_t* handle);
```

`struct liara_renderer_t` is never defined in a public header. Its definition lives in the renderer's private implementation, which is free to put whatever C++ class, state or data it wants behind it.

That buys three things. The implementation may be in any language, so a Rust renderer returning a handle that is internally a `Box<Renderer>` works and the C side never finds out. The size and layout may change between versions without breaking a consumer, because a consumer only ever sees the pointer. And lifetime is explicit, since a handle comes from a `_create` and goes to a `_destroy`, with nothing implicit anywhere.

## Plain-data structs

Data crossing without ownership concerns travels in plain structs: render packets, transforms, events, configuration. They hold fields and nothing else, which C enforces anyway. They use explicit fixed-size types. They are laid out for natural alignment, with no padding hole that varies by platform. And every field is documented in the header.

```c
typedef struct liara_transform_t {
    double   position[3];   // World-space position, double precision.
    float    rotation[4];   // Quaternion (x, y, z, w).
    float    scale[3];      // Per-axis scale.
} liara_transform_t;
```

The position is a `double[3]` rather than a `liara_vec3d_t` for two reasons: it keeps the struct trivially memcpy-able, and it avoids forcing a consumer to know about a maths type it may have no other use for.

That type arrives with v0.2, alongside the rest of the maths types. The double precision in it is the decision recorded under [cross-platform strategy](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/guides/architecture/cross-platform/), taken now because changing a transform's layout later would be a major bump and a migration for everyone.

## Strings

A string is a `const char*`, with an explicit length where it matters.

A null-terminated string is accepted for a short fixed identifier, meaning an asset name or a tag. Anything that may contain text somebody typed is passed as a pointer and a length, which removes the ambiguity about embedded nulls and supports a non-null-terminated slice coming from another language.

```c
// An identifier
liara_result_t liara_core_get_setting(const char* key, ...);

// Arbitrary text
liara_result_t liara_core_log_message(const char* text, size_t length, ...);
```

A string crossing the boundary is borrowed for the duration of the call. The implementation does not retain the pointer afterwards, and it copies when it needs to keep the contents.

## Arrays

An array is a pointer and a count, always.

```c
liara_result_t liara_renderer_submit_packet(
    liara_renderer_handle_t* renderer,
    const liara_drawable_t* drawables,
    size_t drawable_count
);
```

No variable-length array in a struct either. A struct referencing an array does it with the same pair.

```c
typedef struct liara_render_packet_t {
    const liara_view_t*     views;
    size_t                  view_count;
    const liara_drawable_t* drawables;
    size_t                  drawable_count;
    // ...
} liara_render_packet_t;
```
