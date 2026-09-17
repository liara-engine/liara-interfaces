---
title: Changing an interface
description: Five steps, the one place where guessing wrong is asymmetric, and what compatibility is actually promised across versions.
sidebar:
  order: 12
---

## The five steps

**Justify it.** Open an issue here describing the change, the use case driving it, and which component it bumps. The discussion happens on the issue, before any code is written.

**Determine the bump**, from [the table](./breaking-changes/). When in doubt, treat the change as more breaking than it looks. The two errors are not symmetric: a minor bump that should have been major breaks consumers silently, and a major bump that could have been minor costs one version number.

**Open the pull request.** It carries the header changes, regenerated ABI layout assertions for any struct added or modified, and a conventional-commit title that declares the bump, with a `BREAKING CHANGE` footer when the change is one.

The version macros are not edited. `LIARA_ABI_VERSION` is [generated from the project version](./versioning/#the-abi-version-is-generated), so the bump is declared through the commit and applied by release-please at release time. The `abi-snapshot` job computes the bump the diff actually requires and fails the pull request when the declared type does not cover it.

**Coordinate the consumers.** A minor or patch bump needs nothing from them, since existing consumers keep working.

A major bump needs every consumer updated. The pull request description lists what each will require, and the consumer pull requests are opened at the same time, with this one merging last so that nothing sits broken in between. Major bumps are batched: several breaking changes are collected and released together, so that consumers migrate once rather than three times.

**Tag and release.** release-please produces the release from the merged commits, and consumer modules update their pin in their own next release.

## What is promised

Within a major version, two guarantees hold.

Code compiled against version N.x keeps compiling and linking against N.y for any y at or above x.

A module compiled against N.x stays loadable by a host built against N.y for any y at or above x, subject to the `DEGRADED` outcome when the host is the newer of the two and calls something the module predates.

Across major versions, nothing is promised. A module built against 2.0 cannot be loaded by a host built against 1.x, and the reverse fails the same way, which is [`INCOMPATIBLE`](./negotiation/#four-outcomes) refusing before anything is called rather than something crashing later.

Which versions of a module exist, and which interface versions each of them works with, is in that module's `manifest.json`. There is no separate compatibility file, and [ADR 0006](https://liara-engine.liara-engine-documentation.workers.dev/liara/latest/guides/adr/0006-manifest-as-compatibility-source-of-truth/) says why.
