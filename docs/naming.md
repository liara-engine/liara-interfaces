---
title: Naming
description: Prefixed, lowercase, underscore-separated, and the reason C leaves no alternative.
sidebar:
  order: 1
---

Names are prefixed, lowercase and underscore-separated, and the prefix says which module the symbol belongs to.

## Prefixes

| Prefix             | Module                |
|--------------------|-----------------------|
| `liara_`           | Generic, cross-module |
| `liara_core_`      | Core                  |
| `liara_platform_`  | Platform              |
| `liara_renderer_`  | Renderer              |
| `liara_assets_`    | Assets                |
| `liara_audio_`     | Audio                 |
| `liara_physics_`   | Physics, post-v1.0    |
| `liara_scripting_` | Scripting, post-v1.0  |

The bare `liara_` prefix is reserved for what belongs to no single module: the version macros, the result type, the math types, the allocator interface, and similar fundamentals.

## Symbols

Functions are `verb_noun`, prefixed by module.

```c
liara_renderer_create(...)
liara_core_world_create(...)
liara_core_entity_destroy(...)
```

Constants and enum values are uppercase and prefixed.

```c
LIARA_RESULT_SUCCESS
LIARA_RESULT_OUT_OF_MEMORY
LIARA_RENDERER_TARGET_SWAPCHAIN
LIARA_RENDERER_TARGET_OFFSCREEN
```

Macros are uppercase, prefixed, and used sparingly.

```c
LIARA_VERSION_MAJOR
LIARA_MAKE_VERSION_UNSAFE(major, minor, patch)
```

## Handles carry two names

An opaque handle is a pointer to an incomplete struct, and two names are involved. The struct tag names the thing, and the typedef names its type.

```c
LIARA_TYPEDEF(struct liara_renderer_t, liara_renderer_handle_t);
```

The tag is `<module>_t` and the typedef is `<module>_handle_t`, so a consumer passes a `liara_renderer_handle_t*` and a creation function fills a `liara_renderer_handle_t**`.

The struct is never defined in a public header, so a consumer can hold and pass a handle without being able to construct, copy or inspect one, which is the entire point. Adding a field to the definition on the implementation side is not an ABI change, because no consumer's compiler ever knew the size.

## Version words

They are built with `LIARA_MAKE_VERSION_UNSAFE(major, minor, patch)`. The suffix is not decoration: the macro range-checks nothing, and a component overflowing its field corrupts its neighbors silently. `liara_try_make_version()` is the checked form, and it is what anything handling computed or untrusted input calls.

## Why lowercase, why prefixed

C has one global namespace. Without a prefix, two libraries exposing a function called `create` collide at link time, and the verbosity is what namespace safety costs in this language.

Lowercase is the convention every C ecosystem shares, POSIX, Vulkan, SDL and libcurl included. It is followed for consistency with them, regardless of any preference for CamelCase that exists on the C++ side of the boundary.
