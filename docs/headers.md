---
title: Header organization
description: One concept per header, standalone by construction, and the three macro families that make the whole thing consumable from any language with a C FFI.
sidebar:
  order: 9
---

## One concept per header

Each header under `include/liara/` covers one concept, and none aggregates unrelated ones. Wanting to put two unrelated things in the same header is the signal to create two headers.

A module subdirectory (`liara/core/`, `liara/renderer/`) says which module owns the symbols, not which one consumes them. `liara/renderer/packet.h` defines the render packet, and both the core, which produces packets, and the renderer, which consumes them, include it.

## Standalone by construction

A header includes everything it needs and defines everything it declares, so that including exactly one header is enough to use one concept, with no include order to know about.

Forward declarations avoid pulling in a header that is only transitively necessary. A header that needs a type's definition rather than a pointer to it includes the header defining that type.

Include guards are `#pragma once`, everywhere. Every modern compiler supports it, in C as much as in C++, and it is shorter and harder to get wrong than a guard macro somebody has to keep unique.

## `extern "C"`

Every header is wrapped, so a C++ consumer includes it normally and gets C linkage.

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

It is mandatory, and the interface-rules lint in CI checks it rather than leaving it to review.

## Macros do not cross an FFI

A consumer language that parses C headers, meaning Rust through bindgen, Zig through translate-c, or Python through ctypes and hand-written bindings, either does not see macros at all or sees only the simplest constant ones. So where a header would naturally reach for a macro, it reaches for something else first.

An `enum` for an integer constant, which every binding tool sees. A `static inline` function for a computation, compiled into each translation unit, exporting no symbol and staying header-only. Or both, when the macro form is genuinely needed for constant expressions such as a case label, an array size or a `static_assert`, in which case the `static inline` delegates to the macro.

The runtime cost of a `static inline` is zero, since it inlines, and the compile-time cost is negligible. What it buys is that the whole interface is consumable from any language with a C FFI rather than only from C and C++, which is the property the Zig and Rust tests exist to verify.

## Internal macros

Headers under `liara/internal/` are part of this repository's own implementation. They exist so the public headers can spell portability differences in one place, and they carry no stability promise of their own beyond the versioning rules.

A module includes them transitively, by including a public header. It never includes one directly, and never uses the macros they define in its own implementation code. A macro a module legitimately needs is not internal, and belongs in a public header instead.

`liara/internal/portability.h` is where the C-versus-C++ and compiler-versus-platform `#ifdef` ladders live, so that nothing else has to repeat them. It currently provides `LIARA_STATIC_ASSERT` (C11's `_Static_assert` against C++'s `static_assert`), `LIARA_API_DEPRECATED` (`[[deprecated]]` against `__declspec` and `__attribute__`), `LIARA_STATIC_CAST`, `LIARA_NULL`, and the `LIARA_TYPEDEF` family that declares a typedef once for both languages, optionally deprecated and optionally with a struct definition.

Any header needing to spell something differently depending on the language or the standard version routes it through a macro here rather than deriving the `#ifdef` locally. That is the whole point: there is one ladder to get wrong instead of twenty.

## Export macros

Every declaration a module implements carries that module's export macro, `LIARA_<MODULE>_API`, declared in `liara/<module>/<module>_export.h`.

Each expands to `__declspec(dllexport)` or `__declspec(dllimport)` on Windows and to `__attribute__((visibility("default")))` elsewhere, selected by two macros the build defines: `LIARA_<MODULE>_SHARED`, meaning the module is built or consumed as a shared library, and `LIARA_<MODULE>_BUILD`, meaning this translation unit is building the module rather than consuming it.

Modules compile with hidden default visibility, so a declaration that does not carry the macro does not leave the library at all. That is what the `-link` preset catches: a missing export macro produces a link error there and nowhere else.

One export header per module is the rule. A new module adds `liara/<module>/<module>_export.h` rather than reusing another module's macro, because the exporting side and the importing side have to be distinguishable per library.
