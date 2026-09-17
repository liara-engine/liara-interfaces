---
title: About Liara Interfaces
description: C ABI headers shared by every Liara Engine module.
sidebar:
    order: 0
---

# liara-interfaces

> The C ABI headers every Liara Engine module implements or consumes.

This repository defines the contract between modules of the [Liara Engine](https://github.com/liara-engine/liara). It holds C headers and nothing else: types, function declarations, version macros, result codes. No implementation lives here.

Every module depends on it, and on nothing else. A breaking change here forces a migration everywhere at once, which is why the rules for evolving these headers are stricter than for anything else in the project.

## Status

**Phase 0.** The header set is incomplete and changes freely. Under the 0.0.x compatibility rule, a consumer pinned to one patch version does not accept another.

## Documentation

- **[Interface design rules](https://liara-engine.liara-engine-documentation.workers.dev/liara-interfaces/latest/guides/)**: required reading before modifying any header here.
- **[API reference](https://liara-engine.liara-engine-documentation.workers.dev/liara-interfaces/latest/api/)**: generated from the headers.
- **Architecture**: the meta repository's [architecture](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/guides/architecture/) and [modules](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/guides/modules/) pages.
- **Workflow**: [contributing](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/guides/contributing/), and [changing an interface](https://liara-engine.liara-engine-documentation.workers.dev/liara-interfaces/latest/guides/process/) for anything that touches this repository.

## Building it on its own

Usually you do not. This repository is consumed through the workspace superbuild, or through `find_package(LiaraInterfaces)` from an install.

It is also the one module that builds alone without anything else installed, since it depends on nothing. What it does not carry is a `CMakePresets.json`: presets are generated into the workspace from a template in the meta repository, so a bare clone has no preset to name.

```bash
git clone https://github.com/liara-engine/liara-interfaces.git
cd liara-interfaces

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

The Zig and Rust cross-language tests register only if those toolchains are on `PATH`, and are silently skipped otherwise.

The normal setup, with the workspace and the presets, is in [bootstrap](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/guides/bootstrap/).

## Consuming it

```cmake
find_package(LiaraInterfaces REQUIRED)
target_link_libraries(my_module PRIVATE Liara::Interfaces)
```

Nothing goes in the consumer's `vcpkg.json`. This is an `INTERFACE` library with no dependency of its own, resolved through the workspace build or through the CMake package config.

## License

[MIT](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/about/license/).
