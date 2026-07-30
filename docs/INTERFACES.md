# Interfaces

> Rules for designing, implementing, and evolving the C ABI interfaces
> that connect Liara's modules. This document lives in the
> `liara-interfaces` repository and is **required reading** before
> modifying anything in this repository.
>
> The philosophy behind these rules is documented in section 5 of
> the meta repository's [`ARCHITECTURE.md`](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/book/ARCHITECTURE).

---

## 1. Why These Rules Exist

The interfaces in this repository are the contract between modules.
Every module of the engine — the core, the renderer, the editor,
hypothetical third-party reimplementations — communicates through
these headers. A change to an interface ripples through every consumer.

The rules below exist to make this contract **stable, language-agnostic,
and explicit**. They are not aesthetic preferences. Each rule has a
concrete reason, usually one of:

- **ABI stability**: the rule prevents breakage when modules are built
  with different compilers, standard library versions, or build flags.
- **Cross-language portability**: the rule allows a module to be
  reimplemented in another language (Rust, Zig, etc.) without
  contorting the binding.
- **Versioning discipline**: the rule makes it possible to evolve the
  interface without silently breaking consumers.
- **Memory safety**: the rule prevents lifetime confusion and
  ownership ambiguity.

When a rule conflicts with convenience, the rule wins. Convenience can
live behind the boundary, in C++ wrappers and helper headers; the
boundary itself stays disciplined.

---

## 2. The Boundary: What Counts as an Interface

Anything in `liara-interfaces/include/` is an interface. Anything
outside this directory is not.

Headers in this directory must be **valid C** (not just valid C++).
They must compile with a C compiler with no extensions enabled. They
must be includable from C, C++, or any other language with a C FFI.

To enforce this, the repository's CI compiles every header standalone
with both a C compiler (in C11 mode) and a C++ compiler (in C++20
mode), and verifies that no warnings are produced.

The headers may use:

- Fixed-width integer types from `<stdint.h>` (`uint32_t`, `int64_t`,
  etc.)
- Size and pointer-difference types from `<stddef.h>` (`size_t`,
  `ptrdiff_t`, `NULL`)
- Boolean type from `<stdbool.h>` (`bool`, `true`, `false`)

The headers may **not** use:

- C++ types: `std::*`, classes, templates, references, namespaces.
- Platform headers: no `<windows.h>`, no `<unistd.h>`, no `<vulkan.h>`.
  If a platform-specific type must cross the boundary (e.g., a window
  handle), it crosses as an opaque pointer or as a fixed-width integer
  whose interpretation is documented.
- C standard library functions: no `<stdio.h>`, no `<stdlib.h>` calls
  in header bodies. Inline functions in headers do not call into the
  C standard library.
- Variable-length arrays, flexible array members, or other C99/C11
  features that have spotty MSVC support.

---

## 3. Naming Conventions

Names in interfaces are **prefixed**, **lowercase**, and
**underscore-separated**. The prefix identifies the module the symbol
belongs to.

### Prefixes

| Prefix             | Module               |
|--------------------|----------------------|
| `liara_`           | Generic, cross-module|
| `liara_core_`      | Core module          |
| `liara_renderer_`  | Renderer module      |
| `liara_physics_`   | Physics (post-v1.0)  |
| `liara_scripting_` | Scripting (post-v1.0)|

The `liara_` prefix is reserved for types and functions that are not
specific to any single module: the version macros, the result type,
the math types, the allocator interface, and similar fundamentals.

### Symbol Naming

Functions are `verb_noun` and prefixed with the module:

```c
liara_renderer_create(...)
liara_core_world_create(...)
liara_core_entity_destroy(...)
```

Types are nouns and prefixed with the module:

```c
typedef struct liara_renderer_handle_t    liara_renderer_handle_t;
typedef struct liara_core_world_handle_t  liara_core_world_handle_t;
typedef struct liara_view_t             liara_view_t;
```

Type names use a `_t` suffix when the type is a value type meant to be
embedded in structs or passed by value (`liara_view_t`,
`liara_transform_t`). Opaque handle types use a `_handle_t` suffix and
are forward-declared structs whose definition is private to the
implementing module.

Constants and enum values are uppercase, prefixed:

```c
LIARA_RESULT_SUCCESS
LIARA_RESULT_OUT_OF_MEMORY
LIARA_RENDERER_TARGET_SWAPCHAIN
LIARA_RENDERER_TARGET_OFFSCREEN
```

Macros are uppercase, prefixed, and used sparingly:

```c
LIARA_VERSION_MAJOR
LIARA_MAKE_VERSION(major, minor, patch)
```

### Why Lowercase, Why Prefixed

C has a single global namespace. Without prefixes, two libraries that
expose a function called `create` will collide at link time. The
prefix is verbose, but it is the price of namespace safety in C.

Lowercase is the standard convention in C ecosystems (POSIX, Vulkan,
SDL, libcurl). It is followed for consistency with the broader
ecosystem, regardless of any aesthetic preference for camelCase or
PascalCase that may exist in C++ contexts.

---

## 4. Type Conventions

### Fixed-Width Integers Only

Where size matters (struct layouts, serialization, ABI), only
fixed-width types are used. Plain `int`, `long`, and `short` are
forbidden in interface signatures because their size varies across
platforms.

```c
// Good
uint32_t width;
int64_t timestamp_ns;

// Bad
unsigned width;
long timestamp;
```

The exception is `bool`, which is allowed and is treated as
implementation-defined size. Code that needs a specific size for
booleans uses `uint8_t` instead.

### Opaque Handles

Objects owned by a module are exposed to other modules as opaque
handles. The handle is a pointer to a forward-declared struct whose
definition is private:

```c
// In liara/renderer/renderer.h
typedef struct liara_renderer_handle_t liara_renderer_handle;

liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,
    liara_renderer_handle_t** out_handle
);

void liara_renderer_destroy(liara_renderer_handle_t* handle);
```

The struct `liara_renderer_handle_t` is never defined in the public
header. Its definition lives inside the renderer module's private
implementation, where the renderer is free to put whatever C++ class,
state, or data it needs.

This pattern accomplishes three things:

- The implementation may be in any language: a Rust renderer can
  return a handle that is internally a `Box<Renderer>`, and the C
  interface does not care.
- The size and layout of the implementation may change between
  versions without breaking consumers, because consumers only ever
  see the pointer.
- Lifetime is explicit: handles are created by `_create` functions
  and destroyed by `_destroy` functions, with no implicit destruction.

### POD Structs for Data

Plain-old-data structs are used for data that crosses the boundary
without ownership concerns: render packets, transforms, events,
configuration. These structs:

- Have only fields, no methods (impossible in C anyway).
- Have explicit, fixed-size types.
- Are designed for natural alignment (no padding holes that vary by
  platform).
- Are documented field-by-field in the header.

Example:

```c
typedef struct liara_transform_t {
    double   position[3];   // World-space position, double precision.
    float    rotation[4];   // Quaternion (x, y, z, w).
    float    scale[3];      // Per-axis scale.
} liara_transform_t;
```

The position is `double[3]` rather than a `liara_vec3d_t` to keep the
struct trivially memcpy-able and to avoid forcing consumers to know
about a math type they may not otherwise need.

### Strings

Strings are `const char*` with explicit length parameters where
applicable. Null-terminated strings are accepted for short, fixed
identifiers (asset names, tags). Anything that may contain
user-provided text is passed as a `(pointer, length)` pair to avoid
ambiguity about embedded nulls and to support non-null-terminated
slices from other languages.

```c
// Good: identifier
liara_result_t liara_core_get_setting(const char* key, ...);

// Good: arbitrary text
liara_result_t liara_core_log_message(const char* text, size_t length, ...);
```

Strings passed across the boundary are **borrowed for the duration of
the call**. The implementation must not retain the pointer after the
call returns. If the implementation needs to keep the string, it must
copy it.

### Arrays

Arrays are passed as `(pointer, count)` pairs:

```c
liara_result_t liara_renderer_submit_packet(
    liara_renderer_handle_t* renderer,
    const liara_drawable_t* drawables,
    size_t drawable_count
);
```

Variable-length arrays in structs are not used. Where a struct needs
to reference an array, it does so with a `(pointer, count)` pair:

```c
typedef struct liara_render_packet_t {
    const liara_view_t*     views;
    size_t                  view_count;
    const liara_drawable_t* drawables;
    size_t                  drawable_count;
    // ...
} liara_render_packet_t;
```

---

## 5. Error Handling

### The Result Type

Functions that can fail return a `liara_result_t`:

```c
typedef int32_t liara_result_t;
```

Result codes are defined as an `enum` (using X-macros for maintainability):

```c
typedef enum {
    LIARA_RESULT_SUCCESS = 0,
    LIARA_RESULT_<ERROR_NAME_N> = -<N>,
    // ...
} liara_result_t;
```

Codes are sorted by category. For exemple, -2 to -99 are reserved for generic errors (out of memory, invalid
argument, etc.).

`LIARA_RESULT_SUCCESS` is always zero, so `if (result)` reads as "if
there was an error", consistent with most C error handling.

Functions that cannot fail return `void`. Returning `liara_result_t`
unconditionally adds noise; functions should accurately advertise
whether they can fail.

### Output Parameters

Functions that produce values pass them through output parameters,
not return values:

```c
liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,
    liara_renderer_handle_t** out_handle
);
```

The `out_` prefix on parameters is mandatory. Output parameters may
not be `NULL` (it is undefined behavior to call with `out_handle ==
NULL`); functions assume their out parameters are valid pointers.

### No Exceptions, No Longjmp

Implementations may use exceptions, panics, or longjmp internally,
but these must not cross the boundary. A C++ implementation that
catches its exceptions at the boundary and translates them to
`liara_result_t` is conforming; an implementation that lets a C++
exception propagate out of an interface function is non-conforming
and undefined.

The renderer in particular is required to handle Vulkan errors and
GPU exceptions internally and report them as `liara_result_t` values
or, for asynchronous failures, as callbacks (see section 7).

---

## 6. Memory Ownership

Ownership rules are explicit and uniform.

### Caller Allocates Inputs

When a function takes a struct by pointer for input, the caller owns
the memory:

```c
liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,  // Caller allocates and frees.
    liara_renderer_handle_t** out_handle
);
```

The implementation reads from the struct during the call and does not
retain the pointer.

### Module Allocates Its Own State

Functions that produce a handle (`liara_renderer_create`,
`liara_core_world_create`) allocate the underlying state. The state
is freed by the corresponding destroy function:

```c
liara_renderer_handle_t* renderer = NULL;
liara_renderer_create(&info, &renderer);
// ... use renderer ...
liara_renderer_destroy(renderer);
```

The module is free to use any allocator internally. The caller does
not see and does not care.

### Caller-Supplied Allocator (Optional)

For modules where the caller wants control over allocation (typical
in games), the create function may accept a caller-supplied
allocator:

```c
typedef struct liara_allocator_t {
    void* (*alloc)(size_t size, size_t alignment, void* user_data);
    void  (*free)(void* ptr, void* user_data);
    void* user_data;
} liara_allocator_t;

liara_result_t liara_renderer_create(
    const liara_renderer_create_info_t* create_info,
    const liara_allocator_t* allocator,  // May be NULL for default.
    liara_renderer_handle_t** out_handle
);
```

If the caller passes `NULL`, the module uses its default allocator
(typically `malloc`/`free`). If the caller passes a custom allocator,
the module uses it for all allocations associated with the returned
handle. The allocator pointer must remain valid for the lifetime of
the handle.

This pattern is **optional per module**. Modules where allocation is
not performance-critical may not bother. The renderer, the ECS, and
the asset manager are expected to support custom allocators; the
logger and settings are not.

### Borrowed References

When a function gives the caller a pointer that the caller does not
own (e.g., a pointer to internal data), the documentation makes this
explicit:

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

Borrowed pointers have a documented lifetime. After that lifetime
ends, dereferencing is undefined.

---

## 7. Callbacks

Modules may register callbacks with each other. Callbacks have
explicit signatures and are passed alongside a `void* user_data`
that the module passes back when invoking:

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

Rules for callbacks:

- Callbacks are invoked synchronously by the module unless documented
  otherwise.
- Callbacks must not call back into the module that invoked them in
  ways that would cause re-entrancy. Each callback's documentation
  states what is and is not safe.
- Callbacks must not throw exceptions or use longjmp. They follow
  the same rules as interface functions.
- The `user_data` pointer is opaque to the module; it is the caller's
  responsibility to ensure it remains valid for the callback's
  lifetime.

---

## 8. Versioning

### The Version Format

A version is a single 32-bit integer with bit-packed major, minor,
and patch components:

```c
// In liara/version.h
#define LIARA_MAKE_VERSION_UNSAFE(major, minor, patch) \
    (((uint32_t)(major) << 22) | ((uint32_t)(minor) << 12) | (uint32_t)(patch))

#define LIARA_VERSION_MAJOR(version)  (((uint32_t)(version) >> 22) & 0x3FFu)
#define LIARA_VERSION_MINOR(version)  (((uint32_t)(version) >> 12) & 0x3FFu)
#define LIARA_VERSION_PATCH(version)  ((uint32_t)(version) & 0xFFFu)
```

This format is identical to Vulkan's `VK_MAKE_VERSION` and provides
1024 majors, 1024 minors, and 4096 patches. More than enough for any
realistic timeline.

### Version Encoding

A version is a single `uint32_t` packing three components, following
Vulkan's approach: **10 bits major, 10 bits minor, 12 bits patch**
(major and minor 0–1023, patch 0–4095). The bit layout, masks, and
shifts are defined in `liara/version.h`, along with `LIARA_STATIC_ASSERT`
guards ensuring the components never overflow their fields.

Two forms build a packed version:

- `LIARA_MAKE_VERSION_UNSAFE(major, minor, patch)` — a macro, usable
in constant expressions (static_assert, case labels, array sizes).
It performs no range checking; the caller guarantees the ranges.

- `liara_try_make_version(major, minor, patch, out_version)` — a
checked static inline function returning a `liara_result_t`; it
rejects out-of-range components and a NULL output. Preferred
outside constant contexts.

### The ABI Version

The current ABI version is not hand-written as literals. It is
declared in `liara/abi_version.h` as an `enum` whose members are
fed from CMake-generated macros (`LIARA_PRIVATE_CMAKE_VERSION_*`,
produced into `config.h` from the `liara-interfaces` project version):

```c
enum {
    LIARA_ABI_VERSION_MAJOR = LIARA_PRIVATE_CMAKE_VERSION_MAJOR,
    LIARA_ABI_VERSION_MINOR = LIARA_PRIVATE_CMAKE_VERSION_MINOR,
    LIARA_ABI_VERSION_PATCH = LIARA_PRIVATE_CMAKE_VERSION_PATCH,
};

#define LIARA_ABI_VERSION \
    (uint32_t)LIARA_MAKE_VERSION_UNSAFE( \
        LIARA_ABI_VERSION_MAJOR, LIARA_ABI_VERSION_MINOR, LIARA_ABI_VERSION_PATCH)
```
A single source of truth (the project version) drives both the released
package version and the compiled-in ABI constant, so they can never drift.
Every module that consumes the interface captures LIARA_ABI_VERSION at its
own compile time.

### Module Self-Description

Every module exposes a single entry point describing itself:

```c
typedef struct liara_module_info_t {
    uint32_t    struct_version;   // = LIARA_MODULE_INFO_VERSION_1
    uint32_t    abi_version;      // the ABI the module was compiled against
    uint32_t    module_version;   // the module's own version
    const char* name;             // human-readable module name
    // ...
} liara_module_info_t;

#define LIARA_MODULE_INFO_VERSION_1 1u

const liara_module_info_t* liara_<module>_info(void);
```

The distinction between the two version fields matters. `abi_version` is the
interface contract the module was built against — the field a host checks
first, before trusting anything else the module reports. `module_version` is
the module's independent semantic version, tracked per repository and surfaced
to users; it plays no role in compatibility.

Contract of `liara_<module>_info()`:

- Returns a pointer to a **static instance with process lifetime**. The caller
does not own it and must never free it. The function never returns NULL.

- `name` points to a static string literal, borrowed for the module's lifetime;
the caller must not free it.

- The struct is a **versioned struct** (section 8, "Versioned Structs"): `struct_version`
is its first field, and appending fields in a later minor version keeps older callers
binary-compatible.

- The struct carries **data only**. Function pointers are never added to it — dispatch
across the boundary goes through named `extern "C"` functions, not through a table
(see the "vtable structs" anti-pattern, section 14).

For a fast pre-check that reads nothing but the ABI word, a scalar accessor is also
provided and kept even when info() exists:

```c
uint32_t liara_<module>_abi_version(void);
```

A host composing modules reads each module's `info()`, checks `abi_version` against
its own `LIARA_ABI_VERSION`, and wires together only compatible modules. This is the
machine-readable counterpart to the `abi_compatibility` lists published in each
module's `manifest.json`.

### Compatibility Rule

Compatibility is decided by comparing a provided version against a required one.
The canonical helper lives in `liara/abi_version.h`:

```c
// Does `provided` satisfy what `required` asks for?
liara_version_compat_t liara_version_provides(uint32_t provided, uint32_t required);
```

The rule it implements:

- **Different major → never compatible.** Major encodes breaking change;
no minor or patch can bridge it.

- **Required is a 0.0.x version → exact equality required.** Pre-0.1 is
the Phase 0 playground (see `ROADMAP.md` §3): nothing is promised, so
only an identical version satisfies. This is the one place the rule departs
from ordinary semver, and it is deliberate.

- **Otherwise → provided.minor >= required.minor.** Minor bumps are additive,
so a newer provider satisfies an older requirement; patch is ignored.

The argument order is load-bearing and easy to get wrong, which is why the
parameters are named `provided` and `required` rather than two anonymous
`uint32_t`. A convenience wrapper removes the ordering hazard entirely at
the most common call site:

```c
// Is `module_abi` compatible with the ABI this caller was compiled against?
liara_version_compat_t liara_abi_is_compatible(uint32_t module_abi);
```

### Negotiation Outcome

A boolean answers "compatible or not", but a host often needs to distinguish
how compatible, to warn rather than refuse. The negotiation therefore reports
a four-state outcome:

```c
typedef enum liara_version_compat {
    LIARA_VERSION_COMPAT_EXACT        = 0,  // identical
    LIARA_VERSION_COMPAT_COMPATIBLE   = 1,  // provider newer minor, fully usable
    LIARA_VERSION_COMPAT_DEGRADED     = 2,  // provider older minor: some newer functions unavailable
    LIARA_VERSION_COMPAT_INCOMPATIBLE = 3,  // major mismatch, or 0.0.x inequality
} liara_version_compat_t;
```

- `EXACT` / `COMPATIBLE` — proceed. Under `COMPATIBLE`, the provider is a newer
minor than required: everything the host calls exists.

- `DEGRADED` — proceed with care. The provider is an *older* minor than the host
expects; functions added in newer minors may be absent, and the host must check
before calling them. This is the state that gives the "minor mismatch is a warning"
policy its concrete meaning.

- `INCOMPATIBLE` — refuse. Either the majors differ, or a 0.0.x requirement was not met exactly.

- **Patch differences never affect the outcome.** Patch is documentation and comments only.

Composition is the **host's** job (the launcher, later the editor), not the core's: the
core neither loads nor version-checks sibling modules. Today the host calls each module's
entry point directly at static-link time; under future dynamic loading the same host
resolves the entry point via `dlsym`/`GetProcAddress`. The logic is identical; only the
lookup changes.

### What Counts as a Breaking Change

The detailed rules for what triggers a major bump:

| Change                                                                                                            | Bump  |
|-------------------------------------------------------------------------------------------------------------------|-------|
| Removing a function                                                                                               | MAJOR |
| Renaming a function                                                                                               | MAJOR |
| Changing a function's parameter types                                                                             | MAJOR |
| Changing a function's return type (except `void` → `liara_result_t` for a function newly able to fail, see below) | MAJOR |
| Changing the meaning of a parameter                                                                               | MAJOR |
| Removing a struct field                                                                                           | MAJOR |
| Renaming a struct field                                                                                           | MAJOR |
| Changing a struct field's type                                                                                    | MAJOR |
| Reordering struct fields                                                                                          | MAJOR |
| Changing the value of an existing enum constant                                                                   | MAJOR |
| Removing an enum constant                                                                                         | MAJOR |
| Adding a new enum constant in the middle of an enum                                                               | MAJOR |
| Changing the value of an existing macro                                                                           | MAJOR |
| Adding a function                                                                                                 | MINOR |
| Adding a new enum constant at the end of an enum                                                                  | MINOR |
| Adding a new struct                                                                                               | MINOR |
| Adding a new field at the end of an explicitly-versioned struct (see below)                                       | MINOR |
| Documentation, comment, whitespace changes                                                                        | PATCH |

### Versioned Structs (Optional Pattern)

For structs that are likely to grow, an explicit versioning pattern
is used:

```c
typedef struct liara_renderer_create_info_t {
    uint32_t struct_version;  // Always set to LIARA_RENDERER_CREATE_INFO_VERSION_x
    // ... fields ...
} liara_renderer_create_info_t;

#define LIARA_RENDERER_CREATE_INFO_VERSION_1 1
```

The struct's first field is its version. Functions consuming the
struct check the version before accessing fields, so a struct
extended in a minor version (with new fields appended) remains
binary-compatible: the older module sees the version 1 struct and
ignores the new fields.

This pattern is used selectively, only on structs where the
expectation of growth is high. It costs four bytes per struct and a
runtime check, so it is not applied to every struct.

---

## 9. Header Organization

### File-Per-Concept

Each header in `include/liara/` covers one concept. Headers do not
aggregate unrelated concepts; if you find yourself wanting to put two
unrelated things in the same header, create two headers.

The module subdirectories (`liara/core/`, `liara/renderer/`) reflect
which module owns the symbols, not which module consumes them.
`liara/renderer/packet.h` defines render packet structures; both the
core (which produces packets) and the renderer (which consumes them)
include this header.

### Include Discipline

Each header is **standalone**: it includes everything it needs and
defines everything it declares. A consumer should be able to include
exactly one header to use one concept, without needing to know about
include order.

Forward declarations are used to avoid pulling in transitively
unnecessary headers, but a header that uses a type's definition (not
just a pointer to the type) includes the header that defines the
type.

### Include Guards

All headers use `#pragma once`. This is supported by every modern
compiler and is shorter and less error-prone than traditional include
guards. Although the headers are C-callable, `#pragma once` is
universally supported in C compilers as well.

### `extern "C"` Wrapping

Every header is wrapped in `extern "C"` blocks for C++ consumers:

```c
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ... declarations ...

#ifdef __cplusplus
}
#endif
```

This allows C++ consumers to include the header normally and have C
linkage applied. It is mandatory; CI verifies its presence.

### Macros vs. Functions for Cross-Language Consumability

Macros do not cross language FFI boundaries. A consumer language
that parses C headers (Rust via bindgen, Zig via translate-c,
Python via ctypes with manual binding) typically does not see
macros, or sees only the simplest constant ones.
When a header would naturally use a macro for a small computation
or a packed value, prefer instead:

- An `enum` for integer constants (visible to all binding tools).
- A `static inline` function for computations (compiled into each
TU, no symbol exported, header-only-friendly).
- Or both: a macro **and** a `static inline` function delegating
to the macro, when the macro form is needed for constant
expressions (case labels, array sizes, `static_assert`).

The cost of `static inline` functions is zero at runtime (they
inline) and minimal at compile time. The benefit is that the
entire interface is consumable from any language with a C FFI,
not just C and C++.

### Compatibility Macros (`liara/private_utils.h`)

`liara/private_utils.h` is where the C/C++ and compiler/platform
`#ifdef` ladders live so the rest of the headers don't repeat them.
It currently provides things like `LIARA_STATIC_ASSERT` (C11
`_Static_assert` vs. C++ `static_assert`), `LIARA_API_DEPRECATED`
(`[[deprecated]]` vs. `__declspec`/`__attribute__`), `LIARA_API`
(export/import/visibility for the `-dynamic` shared-library preset),
`LIARA_STATIC_CAST`, `LIARA_NULL`, and the `LIARA_TYPEDEF*` family for
declaring a typedef (optionally deprecated, optionally a struct
definition) once for both languages. Any other header that needs to
spell something differently depending on the language or standard
version routes it through a macro here rather than re-deriving the
`#ifdef` locally.

This is deliberately not an exhaustive list, and this section is not
meant to be kept in lockstep with the header: new compatibility
macros get added to `private_utils.h` as new portability gaps turn
up, without requiring an edit here every time. What doesn't change is
that these macros still live at the interface boundary, so the usual
rules apply to them like to any other symbol in this repository:
adding one is a MINOR change, changing an existing macro's expansion
is a MAJOR change (section 8), and the header must keep compiling as
both C11 and C++20 (section 2).

---

## 10. Documentation

Every public symbol in the headers has a Doxygen-style comment
describing:

- What the function does or what the type represents.
- The meaning of each parameter (especially output parameters and
  pointers).
- The return value, if any, and its possible result codes.
- Pre-conditions: what must be true before the call.
- Post-conditions: what is true after the call returns.
- Lifetime and ownership: who owns what, for how long.
- Thread safety: which functions are safe to call concurrently and
  which are not.

Example:

```c
/**
 * @brief Creates a new renderer instance.
 *
 * @param[in] create_info Configuration for the renderer. Must not be NULL.
 *                        Read-only; not retained beyond the call.
 * @param[in] allocator   Optional custom allocator. May be NULL to use
 *                        the module's default allocator. If non-NULL, the
 *                        pointed-to allocator must remain valid for the
 *                        lifetime of the returned handle.
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

The header itself is the primary documentation. Doxygen is run on
the headers to generate browsable HTML, but the header's source is
the canonical reference and is expected to be readable on its own.

---

## 11. Testing

The interface repository contains tests, even though it has no
implementation. The tests verify:

- **Compile-only tests**: each header compiles standalone with both
  a C and a C++ compiler, with strict warning levels enabled, and
  produces no warnings.
- **ABI layout tests**: structs have the expected size and field
  offsets on the supported platforms. These tests use
  `static_assert(sizeof(struct) == expected, ...)` and offsetof
  checks. They catch accidental layout changes.
- **Macro evaluation tests**: version macros and other compile-time
  computations produce the expected values.
- **Cross-language callability tests**: the headers are also exercised
  from Zig and Rust to confirm the interface is consumable through a
  bare C FFI, not only from C and C++.

These tests run in CI on every change. A change that affects layout
or compilation is caught before it merges.

---

## 12. Continuous Integration for the ABI

The checks described in section 11 are not implemented in this
repository. They are reusable workflows maintained in the
`liara-engine/.github` repository (`.github/workflows/reusable-abi-*.yml`,
backed by scripts under `scripts/`), and this repository's
`.github/workflows/ci.yml` calls them pinned to a released tag (e.g.
`@v0`). Bumping to a newer version of the shared checks means bumping
that ref, not just merging something to `.github`'s `main`.

On every push and pull request against `main`, four jobs run in
parallel, followed by an aggregator:

- **`abi-header-portability`** — compiles every public header
  standalone (included twice, to also catch include-guard bugs) as
  both C and C++ across several language standards, warnings as
  errors. This is what actually enforces "valid C, includable from
  C++" (section 2).
- **`abi-interface-rules`** — a libclang-based lint of the structural
  rules from this document: `extern "C"` wrapping, symbol prefixing
  (section 3), fixed-width types only (section 4), `out_`-prefixed
  output parameters (section 5), and forbidden includes (section 2).
- **`abi-layout-freeze`** — regenerates and checks the golden layout
  header (see below), then compiles it across a gcc/clang/MSVC
  matrix to catch layout drift between platforms.
- **`abi-snapshot`** — serializes the public ABI surface to JSON,
  diffs it against the base branch, and classifies the diff using the
  table in section 8 to compute the required version bump. A guard
  step then checks that the PR's declared Conventional Commit type
  (its title, plus a `BREAKING CHANGE` footer) covers at least that
  bump, and fails the PR if it doesn't.
- **`abi-report`** (pull requests only, after the other four) —
  collects their results into a single sticky PR comment. Adding a
  new pipeline step upstream needs no change to this repository; the
  report is driven entirely by the artifacts the other jobs publish.

### Regenerating the Golden Layout Header

`tests/abi/abi_layout.generated.h` is the "golden" file the
`abi-layout-freeze` job checks against: `static_assert`s on the
`sizeof`/`alignof`/`offsetof` of every public POD struct, generated
by `abi_layout_asserts.py`. Regenerate it whenever a change would
alter one of those numbers — a struct added or removed, a field
added, removed, reordered, or retyped. If you forget, CI catches it:
the `--check` step fails with "ABI layout assertions are out of
date" because it re-derives the header in memory and compares it
byte-for-byte against what's committed.

To regenerate locally, run the same script CI runs (it lives in the
sibling `liara-engine/.github` checkout, e.g. `../../.github` from
this workspace) without `--check`, and commit the result alongside
the struct change in the same PR:

```bash
python3 ../../.github/scripts/abi_layout_asserts.py \
    --include-dir include \
    --output tests/abi/abi_layout.generated.h \
    --clang clang-18
```

**Never run `clang-format` on this file.** The CI check is an exact
textual comparison between the script's output and the committed
file, so reformatting it breaks the check on the next regeneration.
Treat it as generated output, not source:
after any bulk-format pass, re-run the generator (or diff against
what CI expects) to make sure it still matches.

---

## 13. Process for Modifying an Interface

Changing an interface is a deliberate act with explicit steps.

### Step 1: Justify the Change

Open an issue in the `liara-interfaces` repository describing the
proposed change, the use case driving it, and which version
component (major, minor, patch) it requires. Discussion happens on
the issue before code is written.

### Step 2: Determine the Bump

Consult the table in section 8. If there is any doubt, treat the
change as more breaking than it appears: a rejected minor bump that
should have been major causes silent breakage in consumers; a
conservative major bump merely costs one version number.

### Step 3: Open a Pull Request

The PR includes:

- The header changes.
- Updates to `LIARA_ABI_VERSION_*` macros in `abi_version.h`.
- New ABI layout tests for any new or modified structs.
- A note in the changelog.

### Step 4: Coordinate with Consumers

For minor and patch bumps, no consumer changes are required: existing
consumers continue to work.

For major bumps, every consumer module needs to be updated. The PR
description lists the consumer changes that will be required, and
the corresponding PRs in consumer modules are opened together. Major
bumps are batched: multiple breaking changes are collected and
released together to minimize the number of major version releases.

### Step 5: Tag and Release

Once merged, the new version is tagged following semantic versioning,
and `release-please` produces the release. Consumer modules update
their version pin in their next release.

---

## 14. Compatibility Across Versions

Within a major version, the interface guarantees that:

- Code compiled against version N.x of the interface continues to
  compile and link against version N.y for any y >= x.
- Modules compiled against version N.x continue to be loadable by a
  core compiled against version N.y for any y >= x (with the warning
  about minor mismatch noted in section 8).

Across major versions, no compatibility is guaranteed. A module
compiled against version 2.0 of the interface cannot be loaded by a
core compiled against version 1.x, and vice versa.

The `manifest.json` file in each module contains a list of all
released versions of this module, along with the list of interface
versions it supports.

---

## 15. Anti-Patterns to Avoid

The following patterns have appeared in early drafts of interfaces in
this project or in similar projects, and are explicitly forbidden.

**Returning C++ types through C wrappers.** A function that returns a
`std::string` wrapped in a custom struct is not C-compatible; the
internal layout of `std::string` differs across standard library
implementations. If a string must be returned, it is returned as
either a writable buffer the caller provides, or as an opaque handle
that the caller queries.

**Inheritance through "vtable" structs.** A struct of function
pointers that simulates virtual dispatch may seem like clean
polymorphism, but it complicates versioning (adding a function
becomes a layout change) and is rarely actually needed at the
boundary. Use opaque handles and module-level dispatch instead.

**Implicit allocation conventions.** A function that "sometimes"
allocates and "sometimes" doesn't, depending on flags or state, is a
source of leaks and double-frees. Each function either allocates or
does not, statically.

**Stateful global functions.** A function whose behavior depends on
hidden global state (besides the module's own internal state) makes
the interface non-thread-safe and difficult to reason about. State
flows through handles, explicitly.

**Versioned function names.** Calling a function `liara_foo_v2`
because `liara_foo` had a breaking change is a workaround that
accumulates cruft. Bump the major version of the interface and rename
back to `liara_foo`. Old code stops compiling, which is the correct
behavior across a major version boundary.

---

## 16. Summary Checklist for New Interface Code

Before submitting interface changes, verify:

- [ ] Header compiles cleanly with both C and C++ compilers.
- [ ] Header is wrapped in `extern "C"` for C++ consumers.
- [ ] Header uses only types from `<stdint.h>`, `<stddef.h>`,
      `<stdbool.h>`, and other interface headers.
- [ ] All new symbols use the correct prefix.
- [ ] Functions that can fail return `liara_result_t`; functions that
      cannot return `void`.
- [ ] Output parameters use the `out_` prefix.
- [ ] Pointer ownership and lifetime are documented for every
      pointer in every signature.
- [ ] String parameters are either short identifiers (null-terminated)
      or arbitrary text (with explicit length).
- [ ] Array parameters use `(pointer, count)` pairs.
- [ ] Every public symbol has Doxygen documentation.
- [ ] If structs were added or modified, ABI layout tests are
      included.
- [ ] Version macros are updated according to section 8's table.
- [ ] Changelog entry is added.
- [ ] If the change is major, consumer module PRs are opened
      simultaneously.

This checklist is repeated in the PR template for the
`liara-interfaces` repository.
