# liara-interfaces

> C ABI headers shared by every Liara Engine module.

This repository defines the contract between modules of the
[Liara Engine](https://github.com/liara-engine/liara). It contains
**only C headers**: types, function declarations, version macros,
result codes. No implementation lives here.

Every module that consumes or implements an interface depends on this
repository. A breaking change here ripples through every consumer, so
the rules for evolving these headers are stricter than for any other
module.

---

## Status

**Phase 0** — bootstrap. The header set is incomplete and may change
freely until the first tagged release.

---

## Documentation

- **Interface design rules** (mandatory reading before modifying any
  header): [`docs/INTERFACES.md`](./docs/INTERFACES.md).
- **Architecture and rationale**: see the meta repository's
  [`docs/ARCHITECTURE.md`](https://github.com/liara-engine/liara/blob/main/docs/ARCHITECTURE.md)
  and [`docs/MODULES.md`](https://github.com/liara-engine/liara/blob/main/docs/MODULES.md).
- **Day-to-day workflow**: meta repository's
  [`docs/CONTRIBUTING.md`](https://github.com/liara-engine/liara/blob/main/docs/CONTRIBUTING.md).
- **API reference** (Doxygen, deployed once Phase 0 completes):
  [liara-engine.github.io/liara-interfaces](https://liara-engine.github.io/liara-interfaces).

---

## Building Standalone

Most of the time you will not build this repository on its own; it is
consumed by other modules through `find_package(LiaraInterfaces)` or
`add_subdirectory()`. To verify the headers compile and the tests pass
in isolation:

```bash
git clone https://github.com/liara-engine/liara-interfaces.git
cd liara-interfaces

cmake --preset=linux-debug-clang
cmake --build --preset=linux-debug-clang
ctest --preset=linux-debug-clang --output-on-failure
```

The full development setup (Vulkan SDK, vcpkg, presets, etc.) is
documented in the meta repository's
[`docs/BOOTSTRAP.md`](https://github.com/liara-engine/liara/blob/main/docs/BOOTSTRAP.md).

---

## Consuming This Library

In a consumer's `CMakeLists.txt`:

```cmake
find_package(LiaraInterfaces REQUIRED)
target_link_libraries(my_module PRIVATE Liara::Interfaces)
```

In a consumer's `vcpkg.json`, no entry is needed: this library is
header-only and resolved through the workspace build or through CMake
package config.

---

## License

Released under the [MIT License](./LICENSE).
