---
title: Memory ownership
description: Who allocates, who frees, what an optional allocator changes, and what a borrowed pointer promises.
sidebar:
  order: 4
---

The rules are uniform across every module, which is most of what makes them usable.

## The caller allocates inputs

A struct passed by pointer for input belongs to the caller.

```c
liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,  // Caller allocates and frees.
    liara_renderer_handle_t** out_handle
);
```

The implementation reads from it during the call and does not retain the pointer. A module that wants to keep something out of an info struct copies it.

## The module allocates its own state

A function producing a handle allocates whatever sits behind it, and the matching destroy function frees it.

```c
liara_renderer_handle_t* renderer = NULL;
liara_renderer_create(&info, &renderer);
// ... use renderer ...
liara_renderer_destroy(renderer);
```

Which allocator it uses internally is its own business, and the caller neither sees it nor cares.

## Unless the caller supplies one

For a module where the caller wants control over allocation, which is normal in a game, the create function may take an allocator.

```c
typedef struct liara_allocator_t {
    void* (*alloc)(size_t size, size_t alignment, void* user_data);
    void  (*free)(void* ptr, void* user_data);
    void* user_data;
} liara_allocator_t;

liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,
    const liara_allocator_t* allocator,  // May be NULL for the default.
    liara_renderer_handle_t** out_handle
);
```

Passing null means the module's default, which is usually `malloc` and `free`. Passing one means the module uses it for every allocation belonging to the returned handle, and the allocator pointer itself has to stay valid for that handle's whole lifetime.

The pattern is optional per module rather than mandatory. The renderer, the ECS and the asset manager are expected to support it; the logger and the settings are not, because nothing about their allocation is worth controlling.

## Borrowed references

When a function hands back a pointer the caller does not own, the documentation says so and says for how long.

```c
/**
 * Returns a pointer to the entity's transform.
 * The pointer is valid until the next call that modifies the world.
 * The caller must not free the returned pointer.
 */
const liara_transform_t* liara_core_entity_get_transform(
    liara_core_world_handle_t* world,
    liara_entity_t entity
);
```

A borrowed pointer has a documented lifetime, and dereferencing it after that lifetime ends is undefined. Writing the lifetime down is the whole mechanism here, since nothing in the C type system can express it.
