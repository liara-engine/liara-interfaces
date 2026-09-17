---
title: Callbacks
description: How a module calls back into its host, the user data pointer that makes it possible, and the four rules that keep it safe.
sidebar:
  order: 5
---

A module may register a callback with another. The signature is explicit, and it always carries a `void* user_data` that the module hands back when it invokes.

```c
typedef void (*liara_renderer_error_callback)(
    liara_result_t error,
    const char* message,
    void* user_data
);

liara_result_t liara_renderer_set_error_callback(
    liara_renderer_handle_t* renderer,
    liara_renderer_error_callback callback,
    void* user_data
);
```

The `user_data` pointer exists because a C function pointer carries no state. Without it, a callback could only reach global state, which is the [anti-pattern](./anti-patterns/) two doors down.

Four rules apply.

A callback is invoked synchronously unless its documentation says otherwise.

A callback does not call back into the module that invoked it in a way that causes re-entrancy, and each callback's documentation states what is and is not safe to call from inside it.

A callback throws nothing and does not `longjmp`. It follows the same rules as an interface function, for the same reason: it runs on the module's stack, and unwinding through that frame is undefined.

The `user_data` pointer is opaque to the module, and keeping it valid for the callback's whole lifetime is the caller's job. A module never inspects it, copies what it points at, or frees it.
