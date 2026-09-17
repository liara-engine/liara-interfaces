---
title: Self-description and negotiation
description: The one entry point every module exports, the compatibility rule applied to what it reports, and the four outcomes a host acts on.
sidebar:
  order: 7
---

## Every module describes itself

One entry point, the same shape in every module.

```c
typedef struct liara_module_info_t {
    uint32_t    struct_version;      // = LIARA_MODULE_INFO_VERSION
    uint32_t    abi_version;         // the ABI the module was compiled against
    uint32_t    module_version;      // the module's own version
    uint32_t    reserved;            // must be zero
    const char* module_name;         // human-readable module name
    const char* abi_version_str;     // "major.minor.patch" of abi_version
    const char* module_version_str;  // "major.minor.patch" of module_version
} liara_module_info_t;

#define LIARA_MODULE_INFO_VERSION 1U

const liara_module_info_t* liara_<module>_info(void);
```

The two version fields are not interchangeable, and confusing them is the mistake this struct exists to prevent. `abi_version` is the contract the module was built against, and it is what a host checks first, before trusting anything else the module reports. `module_version` is the module's own semantic version, tracked per repository and shown to users, and it plays no part in compatibility.

The contract of `liara_<module>_info()`:

It returns a pointer to a static instance with process lifetime. The caller does not own it, never frees it, and never gets null back.

`module_name`, `abi_version_str` and `module_version_str` point at static string literals borrowed for the module's lifetime. The two string forms are redundant with the packed words, and they exist so a host can print a version without unpacking anything.

`reserved` makes the padding before the first pointer explicit rather than leaving it to the compiler, must be zero, is never read, and is where the first small field appended in a later minor version will go.

The struct is itself a [versioned struct](./breaking-changes/#versioned-structs), with `struct_version` first, so appending fields in a later minor keeps older callers binary-compatible.

It carries data only. A function pointer is never added to it, because dispatch across the boundary goes through named `extern "C"` functions rather than a table, for the reasons under [anti-patterns](./anti-patterns/).

A scalar accessor exists alongside it and is kept even though `info()` covers it, for a pre-check that reads nothing but the ABI word:

```c
uint32_t liara_<module>_abi_version(void);
```

## The compatibility rule

Compatibility compares a provided version against a required one, through one function in `liara/abi_version.h`.

```c
// Does `provided` satisfy what `required` asks for?
liara_version_compat_t liara_version_provides(uint32_t provided, uint32_t required);
```

Applied in order:

1. **Identical versions are `EXACT`.** Nothing else is tested.
2. **Different majors are never compatible.** Major encodes breaking change, and no minor or patch bridges it.
3. **If either side is a 0.0.x version, only exact equality is compatible.** Pre-0.1 is the Phase 0 playground and promises nothing in either direction, so a 0.0.x version neither satisfies nor is satisfied by anything but itself.
4. **Otherwise, a provided minor at or above the required minor is `COMPATIBLE`, and below it is `DEGRADED`.** Minor bumps are additive, so a newer provider satisfies an older requirement.

Rule 3 is the one place this departs from ordinary semver, and it is the strictest of the readings available. The alternative, treating a 0.0.x requirement as an ordinary minor comparison, would let a playground version pass silently for a released one, and the failure would surface as undefined behavior at a layout mismatch rather than as a refusal at load time.

The argument order is load-bearing and easy to invert, which is why the parameters are named `provided` and `required` rather than being two anonymous `uint32_t`. At the most common call site a wrapper removes the hazard entirely:

```c
// Is `module_abi` compatible with the ABI this caller was compiled against?
liara_version_compat_t liara_abi_is_compatible(uint32_t module_abi);
```

## Four outcomes

A boolean answers whether two versions are compatible. A host usually needs to know how compatible, so it can warn instead of refusing.

```c
typedef enum liara_version_compat {
    LIARA_VERSION_COMPAT_EXACT        = 0,  // identical
    LIARA_VERSION_COMPAT_COMPATIBLE   = 1,  // provider newer minor, fully usable
    LIARA_VERSION_COMPAT_DEGRADED     = 2,  // provider older minor: some newer functions unavailable
    LIARA_VERSION_COMPAT_INCOMPATIBLE = 3,  // major mismatch, or 0.0.x inequality
} liara_version_compat_t;
```

`EXACT` and `COMPATIBLE` both mean proceed. Under `COMPATIBLE` the provider is a newer minor than required, so everything the host calls exists.

`DEGRADED` means proceed with care. The provider is an older minor than the host expects, so functions added in newer minors may be absent and the host checks before calling them. This is what gives "a minor mismatch is a warning" its concrete meaning.

`INCOMPATIBLE` means refuse. Either the majors differ, or a 0.0.x requirement was not met exactly.

Patch never affects the outcome, except under rule 3. Above 0.0.x it is documentation and comments and is ignored entirely. Under rule 3, equality is exact and therefore includes patch, which is the point: in the playground, two patches apart is as good as unrelated.

## Who runs it

The host, meaning the launcher today and the editor later. The core neither loads nor version-checks a sibling, which is [ADR 0003](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/guides/adr/0003-the-host-composes-modules/).

Both lookup paths exist today and the preset chooses between them. Under a linked build the host calls each module's entry point directly at static-link time. Under the `-runtime` presets it opens the shared libraries with `dlopen` or `LoadLibrary` and resolves `liara_<module>_info` by name. The negotiation logic is identical in both, and only the lookup differs.
