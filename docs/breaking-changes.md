---
title: What breaks, and what does not
description: The change-to-bump table, and the versioned-struct pattern that turns a growing struct from a major bump into a minor one.
sidebar:
  order: 8
---

## The table

| Change                                                                                                          | Bump  |
|-----------------------------------------------------------------------------------------------------------------|-------|
| Removing a function                                                                                             | MAJOR |
| Renaming a function                                                                                             | MAJOR |
| Changing a function's parameter types                                                                           | MAJOR |
| Changing a function's return type, except `void` to `liara_result_t` for a function that newly becomes fallible | MAJOR |
| Changing the meaning of a parameter                                                                             | MAJOR |
| Removing a struct field                                                                                         | MAJOR |
| Renaming a struct field                                                                                         | MAJOR |
| Changing a struct field's type                                                                                  | MAJOR |
| Reordering struct fields                                                                                        | MAJOR |
| Changing the value of an existing enum constant                                                                 | MAJOR |
| Removing an enum constant                                                                                       | MAJOR |
| Adding an enum constant in the middle of an enum                                                                | MAJOR |
| Changing the value of an existing macro                                                                         | MAJOR |
| Adding a function                                                                                               | MINOR |
| Adding an enum constant at the end of an enum                                                                   | MINOR |
| Adding a struct                                                                                                 | MINOR |
| Adding a field at the end of a versioned struct                                                                 | MINOR |
| Documentation, comments, whitespace                                                                             | PATCH |

Two of these rows deserve a note.

**Changing the meaning of a parameter** is a major bump that no tool detects. The signature is unchanged, so the ABI snapshot shows no diff and the layout freeze sees nothing, which leaves the conventional commit and the `BREAKING CHANGE` footer as the entire mechanism. Nothing catches you forgetting.

**`void` to `liara_result_t`** is the one exemption in the table, and it is worth knowing why it is safe before relying on it: a caller compiled against the old signature ignores a return value that now arrives in a register it was already ignoring.

## Versioned structs

For a struct likely to grow, an explicit versioning pattern turns what would be a major bump into a minor one.

```c
typedef struct liara_renderer_create_info_t {
    uint32_t struct_version;  // Always set to LIARA_RENDERER_CREATE_INFO_VERSION_x
    // ... fields ...
} liara_renderer_create_info_t;

#define LIARA_RENDERER_CREATE_INFO_VERSION_1 1
```

The version is the struct's first field, and a function consuming the struct checks it before touching anything after it. A struct extended in a minor version, with the new fields appended, therefore stays binary-compatible: an older module sees version 1 and ignores what it does not know about.

The pattern is applied selectively rather than everywhere. It costs four bytes per struct and a check at run time, which is worth paying only where growth is genuinely expected. `liara_module_info_t` and `liara_render_packet_t` both carry it, for the same reason: both are known to be incomplete today.
