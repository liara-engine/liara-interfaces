---
title: Testing and CI
description: Four kinds of test for a repository with no implementation, the five jobs that run them, and how to regenerate the golden layout header.
sidebar:
  order: 9
---

## What a contract repository tests

There is no implementation here, and there is still plenty to verify.

**Compile-only tests** check that each header compiles standalone under both a C and a C++ compiler, at strict warning levels, with no warning produced.

**ABI layout tests** check that every public struct has the expected size and field offsets on every supported platform, through `static_assert` on `sizeof` and `offsetof`. They are what catches an accidental layout change, which is the failure mode with no other symptom.

**Macro evaluation tests** check that the version macros and the other compile-time computations produce the values they are supposed to.

**Cross-language callability tests** exercise the headers from Zig and Rust, confirming the interface is consumable through a bare C FFI rather than only from the C family. They are what turns "the contract is language-agnostic" from an intention into a property.

## The pipeline

None of those checks is implemented in this repository. They are reusable workflows in `liara-engine/.github`, as `.github/workflows/reusable-abi-*.yml` backed by scripts under `scripts/`, and this repository's `ci.yml` calls them pinned to a released tag. Moving to a newer version of the shared checks means bumping that ref, and not merging something to `.github`'s `main`.

On every push and pull request against `main`, four jobs run in parallel and an aggregator follows.

**`abi-header-portability`** compiles every public header standalone, included twice to catch include-guard bugs, as C and as C++ across several language standards, warnings as errors. This is what actually enforces "valid C, includable from C++".

**`abi-interface-rules`** is a libclang-based lint of the structural rules from these pages: `extern "C"` wrapping, symbol prefixing, fixed-width types only, `out_`-prefixed output parameters, and forbidden includes.

**`abi-layout-freeze`** regenerates the golden layout header, checks it against what is committed, then compiles it across a GCC, Clang and MSVC matrix to catch layout drift between platforms.

**`abi-snapshot`** serialises the public ABI surface to JSON, diffs it against the base branch, and classifies the diff to compute the version bump the change requires. A guard step then checks that the pull request's declared conventional-commit type and `BREAKING CHANGE` footer cover at least that bump, and fails it when they do not.

**`abi-report`** runs on pull requests, after the other four, and collects their results into one sticky comment. Adding a step upstream needs no change here, because the report is driven entirely by the artifacts the other jobs publish.

## Regenerating the golden layout header

`tests/abi/abi_layout.generated.h` is what `abi-layout-freeze` compares against. Regenerate it whenever a change alters one of the numbers in it: a struct added or removed, a field added, removed, reordered or retyped.

Forgetting is caught rather than silent. The `--check` step re-derives the header in memory and compares it byte for byte against what is committed, and fails with "ABI layout assertions are out of date".

Run the same script CI runs, from the sibling `.github` checkout, and commit the result alongside the struct change in the same pull request.

```bash frame="terminal"
python3 ../../.github/scripts/abi_layout_asserts.py \
    --include-dir include \
    --output tests/abi/abi_layout.generated.h \
    --clang clang-20
```

:::danger[Never run clang-format on that file]
The check is an exact textual comparison between the script's output and the committed file, so reformatting it breaks the check at the next regeneration. Treat it as generated output rather than source: after any bulk-format pass, re-run the generator and confirm it still matches.
:::
