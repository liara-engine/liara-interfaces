---
title: Anti-patterns
description: Five shapes that look like reasonable C and are forbidden here, with what each one actually costs.
sidebar:
  order: 11
---

Each of these has appeared in an early draft of an interface, here or in a similar project. None of them is obviously wrong on sight, which is why the list exists.

**Returning a C++ type through a C wrapper.** A function returning a `std::string` inside a custom struct is not C-compatible, because the layout of `std::string` differs between standard library implementations. A string that has to come back does so through a buffer the caller provides, or through an opaque handle the caller queries.

**Simulating inheritance with a vtable struct.** A struct of function pointers looks like clean polymorphism and complicates versioning immediately: adding a function becomes a layout change. It is also rarely needed at this boundary, where opaque handles and module-level dispatch do the same job.

**Allocating conditionally.** A function that sometimes allocates and sometimes does not, depending on a flag or on state, produces leaks and double frees in equal measure. A function either allocates or does not, statically, and its documentation says which.

**Depending on hidden global state.** A function whose behavior depends on something other than its arguments and its module's own state is not thread-safe and cannot be reasoned about locally. State flows through handles, explicitly.

**Versioning a function's name.** Calling something `liara_foo_v2` because `liara_foo` broke is a workaround that accumulates. Bump the interface's major version and keep the name. Old code stops compiling, which is exactly the right behavior across a major boundary, and it is what the version negotiation exists to make survivable.
