---
title: Version encoding
description: One 32-bit word, two ways to build it, and why the ABI version is never typed by hand.
sidebar:
  order: 6
---

## One word

A version is a single `uint32_t` packing three components the way Vulkan packs them: 10 bits of major, 10 of minor, 12 of patch. That gives 1024 majors, 1024 minors and 4096 patches, which outlasts any realistic timeline.

```c title="liara/version.h"
#define LIARA_MAKE_VERSION_UNSAFE(major, minor, patch) \
    (((uint32_t)(major) << 22) | ((uint32_t)(minor) << 12) | (uint32_t)(patch))

#define LIARA_VERSION_MAJOR(version)  (((uint32_t)(version) >> 22) & 0x3FFu)
#define LIARA_VERSION_MINOR(version)  (((uint32_t)(version) >> 12) & 0x3FFu)
#define LIARA_VERSION_PATCH(version)  ((uint32_t)(version) & 0xFFFu)
```

The layout, the masks and the shifts all live in that one header, with `LIARA_STATIC_ASSERT` guards making sure no component can overflow its field.

A word is unreadable in a debugger, which is what `liara_version_to_string` exists for. It should be the only thing decoding one.

## Two ways to build one

`LIARA_MAKE_VERSION_UNSAFE(major, minor, patch)` is a macro, so it works in a constant expression: a `static_assert`, a case label, an array size. It range-checks nothing, and the caller guarantees the ranges. A component that overflows corrupts its neighbors silently, which is what the suffix is warning about.

`liara_try_make_version(major, minor, patch, out_version)` is a checked `static inline` returning a `liara_result_t`. It rejects an out-of-range component and a null output, and it is what anything outside a constant context should call.

The reasoning behind having a packed word at all rather than a string is in [ADR 0005](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/guides/adr/0005-version-encoding-and-compatibility/), along with the divergence between this document and the code that the rule was written to settle.

## The ABI version is generated

The current ABI version is not written as literals anywhere. `liara/abi_version.h` declares it as an enum fed from macros CMake generates into `config.h` out of the `liara-interfaces` project version.

```c title="liara/abi_version.h"
enum {
    LIARA_ABI_VERSION_MAJOR = LIARA_PRIVATE_CMAKE_VERSION_MAJOR,
    LIARA_ABI_VERSION_MINOR = LIARA_PRIVATE_CMAKE_VERSION_MINOR,
    LIARA_ABI_VERSION_PATCH = LIARA_PRIVATE_CMAKE_VERSION_PATCH,
};

#define LIARA_ABI_VERSION \
    (uint32_t)LIARA_MAKE_VERSION_UNSAFE( \
        LIARA_ABI_VERSION_MAJOR, LIARA_ABI_VERSION_MINOR, LIARA_ABI_VERSION_PATCH)
```

One source of truth, the project version, drives both the released package version and the compiled-in constant, so the two cannot drift. Every module consuming the interface captures `LIARA_ABI_VERSION` at its own compile time, which is what makes the later negotiation meaningful: the number a module reports is the contract it was actually built against, not the one it happens to be sitting next to.

The practical consequence for anyone changing an interface: bumping the version is not an edit to a header. It is a release, driven by the conventional commit, and [the process](./process/) says what that looks like.
