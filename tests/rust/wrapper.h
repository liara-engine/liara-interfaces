#ifndef LIARA_RUST_WRAPPER_H
#define LIARA_RUST_WRAPPER_H

#include <liara/version.h>

/* These wrappers force the inline functions to have real exported symbols
   that Rust's FFI can find. */

uint32_t liara_rs_make_version(uint32_t major, uint32_t minor, uint32_t patch);
uint32_t liara_rs_version_major(uint32_t version);
uint32_t liara_rs_version_minor(uint32_t version);
uint32_t liara_rs_version_patch(uint32_t version);
uint32_t liara_rs_interface_version(void);

#endif
