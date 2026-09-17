---
title: Checklist
description: What to verify before submitting an interface change. Repeated in this repository's pull request template.
sidebar:
  order: 12
---

- [ ] The header compiles cleanly with both a C and a C++ compiler.
- [ ] It is wrapped in `extern "C"` for C++ consumers.
- [ ] It uses only types from `<stdint.h>`, `<stddef.h>`, `<stdbool.h>` and other interface headers.
- [ ] Every new symbol carries the right prefix.
- [ ] Functions that can fail return `liara_result_t`; functions that cannot return `void`.
- [ ] Output parameters carry the `out_` prefix.
- [ ] Ownership and lifetime are documented for every pointer in every signature.
- [ ] String parameters are either short identifiers, null-terminated, or arbitrary text with an explicit length.
- [ ] Array parameters are a pointer and a count.
- [ ] Every public symbol has Doxygen documentation.
- [ ] Structs added or modified come with updated ABI layout assertions, regenerated rather than hand-edited.
- [ ] The version macros are updated to match the kind of change.
- [ ] A changelog entry exists, or the conventional commit is correct enough for release-please to write one.
- [ ] For a breaking change, the consumer module pull requests are open at the same time, and this one merges last.

This list is repeated in the pull request template for this repository, which is where it actually gets read.
