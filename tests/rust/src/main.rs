#![allow(non_upper_case_globals, non_camel_case_types, non_snake_case, dead_code)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

fn main() {
    // Test 1: enum constants (these come straight from liara/version.h)
    println!("LIARA_INTERFACE_VERSION_MAJOR = {}", LIARA_INTERFACE_VERSION_MAJOR);
    println!("LIARA_INTERFACE_VERSION_MINOR = {}", LIARA_INTERFACE_VERSION_MINOR);
    println!("LIARA_INTERFACE_VERSION_PATCH = {}", LIARA_INTERFACE_VERSION_PATCH);

    assert_eq!(LIARA_INTERFACE_VERSION_MAJOR, 0);
    assert_eq!(LIARA_INTERFACE_VERSION_MINOR, 0);
    assert_eq!(LIARA_INTERFACE_VERSION_PATCH, 0);

    // Test 2: inline functions, called through the C wrapper
    let v = unsafe { liara_rs_make_version(2, 5, 17) };
    println!("liara_make_version(2, 5, 17) = 0x{:08x} = {}", v, v);

    let major = unsafe { liara_rs_version_major(v) };
    let minor = unsafe { liara_rs_version_minor(v) };
    let patch = unsafe { liara_rs_version_patch(v) };
    println!("decoded: major={}, minor={}, patch={}", major, minor, patch);
    assert_eq!(major, 2);
    assert_eq!(minor, 5);
    assert_eq!(patch, 17);

    // Test 3: roundtrip with the project's actual version
    let project_version = unsafe { liara_rs_interface_version() };
    let pmaj = unsafe { liara_rs_version_major(project_version) };
    println!("project version major = {}", pmaj);
    assert_eq!(pmaj, LIARA_INTERFACE_VERSION_MAJOR as u32);

    println!("\nAll Rust FFI tests passed!");
}
