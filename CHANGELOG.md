# Changelog

## [0.1.0](https://github.com/liara-engine/liara-interfaces/compare/v0.0.1...v0.1.0) (2026-07-18)


### ⚠ BREAKING CHANGES

* **version:** liara_try_make_version now return liara_result, 0 is now a success

### Features

* **abi:** introduce LIARA_STATIC_ASSERT ([f388fe3](https://github.com/liara-engine/liara-interfaces/commit/f388fe3429ea88fe899b20a57e7e1d832f6ec9dc))
* **ci:** add CI workflow ([#6](https://github.com/liara-engine/liara-interfaces/issues/6)) ([e6ea14e](https://github.com/liara-engine/liara-interfaces/commit/e6ea14e59b5a2afc228e05ef0c663d9aba6d11f7))
* **core:** add a first very basic Liara core API ([bb90517](https://github.com/liara-engine/liara-interfaces/commit/bb90517be2297d3e7dcf090826235e8f6c6852c2))
* **manifest:** update ABI compatibility for versions 0.0.1 and 0.1.0 ([cc35622](https://github.com/liara-engine/liara-interfaces/commit/cc3562211d09cd7b8fda82e9df554f4b822f35d5))
* **renderer:** add a first very basic Liara renderer API (console only) ([0809a0a](https://github.com/liara-engine/liara-interfaces/commit/0809a0abf3da9ada61517013f4a817658a9654d4))
* **renderer:** add functions to retrieve renderer version and println ([1f990a9](https://github.com/liara-engine/liara-interfaces/commit/1f990a97c69000f73661534831a29fd161590224))
* **renderer:** update renderer functions to return liara_result for better error handling ([06450ac](https://github.com/liara-engine/liara-interfaces/commit/06450aca44213f3dfdb54831377f70ac0d49797e))
* **result:** add liara_result type and error code definitions ([62815b2](https://github.com/liara-engine/liara-interfaces/commit/62815b279fb4676b1211467ecd59ca238d4f0d81))
* **version:** update liara_try_make_version to return error codes for null pointer and out of range values ([d5fabd6](https://github.com/liara-engine/liara-interfaces/commit/d5fabd6464f9936e64354e099b8dae695eda11ee))


### Bug Fixes

* **format:** update clang-format and clang-tidy ([d8dd48e](https://github.com/liara-engine/liara-interfaces/commit/d8dd48e5f9cd4185224c62fce1c2206cf5528ff5))
* **renderer:** add missing liara result include ([68dfb3c](https://github.com/liara-engine/liara-interfaces/commit/68dfb3c1bfa4a6ed9384b1c5e5cf67feb75f153b))

## 0.0.1 (2026-07-15)


### Features

* **abi:** add Liara ABI version definition and compatibility functions ([ca4e3cc](https://github.com/liara-engine/liara-interfaces/commit/ca4e3cc4a6367a38df0ac2aefa4b1cd6cf094536))
* add versioning macros for Liara project ([224f980](https://github.com/liara-engine/liara-interfaces/commit/224f9809de1f678093f338beaf2e0ff6f53e5c87))
* **docs:** add alias for thread safety in Doxyfile ([4f19c4e](https://github.com/liara-engine/liara-interfaces/commit/4f19c4e86e3e607d2681013287a86bbb16bcc018))
* **docs:** add initial book configuration for user guide ([83167a8](https://github.com/liara-engine/liara-interfaces/commit/83167a89519651d23a67c7b250782eefef0d1b27))
* **docs:** add initial documentation module configuration ([2d16b58](https://github.com/liara-engine/liara-interfaces/commit/2d16b58c00eb7dbb37b2753c6aafcddeb52dadc9))
* **docs:** add project icon and enable code folding in Doxyfile ([cc1bbe8](https://github.com/liara-engine/liara-interfaces/commit/cc1bbe8e33196d823c84d152e5645c8e0d933f01))
* **docs:** add workflows for documentation preview and cleanup on pull requests ([ce72c58](https://github.com/liara-engine/liara-interfaces/commit/ce72c58e728b910823050220d6f7dd3da116cc62))
* enhance versioning system with detailed macros and inline functions ([ad37dbf](https://github.com/liara-engine/liara-interfaces/commit/ad37dbfb41c04bd075f1f3febb26b43f55a7d329))
* **manifest:** add initial module manifest ([f019313](https://github.com/liara-engine/liara-interfaces/commit/f0193131bf09c99854b7ad3faa3f4bd547fa1156))
* **release:** add configuration files for release-please automation ([f11fdf2](https://github.com/liara-engine/liara-interfaces/commit/f11fdf2d7baa8b49ea7a0aade429ef85a58f5138))
* **release:** update release-please configuration for pre-major versioning ([9b6068d](https://github.com/liara-engine/liara-interfaces/commit/9b6068d19a3cc67b603aca50b420ec6056be1bb5))
* **tests:** add platform-specific Zig and Rust test commands ([a4e6ad4](https://github.com/liara-engine/liara-interfaces/commit/a4e6ad4d1ea711ff84519d3336fa58196e36bf60))
* **version:** enhance version handling with safety checks and new comparison functions ([5c1fc9a](https://github.com/liara-engine/liara-interfaces/commit/5c1fc9a393d191e236e7fe84e89a2fdcbcdbb75c))
* **versioning:** automate version management with generated config header ([3cf4e4f](https://github.com/liara-engine/liara-interfaces/commit/3cf4e4f2ea1f9b1df0b709d40d55795d3d8493c8))


### Bug Fixes

* add extern "C" linkage for versioning macros in version.h ([dd50268](https://github.com/liara-engine/liara-interfaces/commit/dd5026895c73392543308e24dcdec2fbd4b6ceff))
* **docs:** update Doxyfile output directory and resource paths ([93f4da2](https://github.com/liara-engine/liara-interfaces/commit/93f4da21cfe17b3543c07499fd3ced514499bac4))
* **docs:** update image reference in documentation workflow to fix a typo ([a343e18](https://github.com/liara-engine/liara-interfaces/commit/a343e18f8c0da1275d93278e8674471932ee98fe))
* **docs:** update links in documentation for consistency and accuracy ([5d4006b](https://github.com/liara-engine/liara-interfaces/commit/5d4006bfdca8083d7c9c97e2880897ba0d888e61))
* **docs:** update permissions for packages in docs-preview.yml ([52e984d](https://github.com/liara-engine/liara-interfaces/commit/52e984d65440399a3c3e58d1526c4830875c244e))
* **docs:** update permissions for preview workflow in docs-preview.yml ([64b842e](https://github.com/liara-engine/liara-interfaces/commit/64b842ebff5403b8613662eb191b7fdc674caa13))
* **docs:** update repository URLs and add navbar configuration file ([6486d07](https://github.com/liara-engine/liara-interfaces/commit/6486d07ea68dcf5760dec466b43c2e62af0bcace))
* **doxygen:** fix thread safety annotations and formatting in Doxyfile ([#2](https://github.com/liara-engine/liara-interfaces/issues/2)) ([d63e9da](https://github.com/liara-engine/liara-interfaces/commit/d63e9dacef718439b9e2d602cb117c22e3cf51cc))
* fix project name and input paths in Doxyfile ([ff77761](https://github.com/liara-engine/liara-interfaces/commit/ff77761f08414a35ce036c97d6115a51209bdccb))
* **test:** update patch version to 1 in each test ([39b5196](https://github.com/liara-engine/liara-interfaces/commit/39b5196b127e317f5ddeda1b73dc83efb1154f93))
* update CMakeLists.txt for conditional C and C++ standard settings ([651fbec](https://github.com/liara-engine/liara-interfaces/commit/651fbec6271ba882d3ef0d2b1aae24c216ad8dad))
