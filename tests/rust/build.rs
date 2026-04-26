use std::env;
use std::path::PathBuf;

fn main() {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    let include_dir = manifest_dir
        .join("../../include")
        .canonicalize()
        .expect("Cannot find liara-interfaces/include directory");

    println!("cargo:rerun-if-changed={}", include_dir.display());
    println!("cargo:rerun-if-changed=wrapper.h");
    println!("cargo:rerun-if-changed=wrapper.c");

    // Compile the wrapper.c into a static library that Rust will link against.
    cc::Build::new()
        .file("wrapper.c")
        .include(&include_dir)
        .flag_if_supported("-std=c11")
        .compile("liara_rs_wrapper");

    // Generate Rust bindings for wrapper.h (which transitively pulls in version.h).
    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .clang_arg(format!("-I{}", include_dir.display()))
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings");
}
