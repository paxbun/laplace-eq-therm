// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

use std::env::var;
use std::fs::File;
use std::io::{Result, Write};
use std::path::Path;

fn main() -> Result<()> {
    // Set profile as an environment variable: used to build tests/import_test
    println!("cargo:rustc-env=PROFILE={}", var("PROFILE").unwrap());
    // Link the C++ standard library
    link_cplusplus();
    // Configure and build the CMake project
    run_cmake("core", "laplace-eq-therm-server-core");
    // Generate bindings from core/Lib.hh
    generate_bindings("core/Lib.hh")
}

#[cfg(target_os = "windows")]
fn link_cplusplus() {
    /* do nothing */
}

#[cfg(target_os = "linux")]
use std::process::Command;

#[cfg(target_os = "linux")]
fn link_cplusplus() {
    // If CXX is set
    if let Ok(var) = var("CXX") {
        // Find where the corresponding version of libstdc++.a is located
        let output = Command::new(var)
            .arg("--print-file-name=libstdc++.a")
            .output()
            .unwrap();

        let path = String::from_utf8_lossy(&output.stdout).to_string();
        let path = Path::new(&path);

        println!(
            "cargo:rustc-link-search=native={}",
            path.parent()
                .expect("Cannot find proper version of libstdc++.a")
                .display()
        );
    }
    println!("cargo:rustc-link-lib=static=stdc++");
}

#[cfg(target_os = "macos")]
fn link_cplusplus() {
    println!("cargo:rustc-link-lib=static=c++");
}

fn run_cmake(source_dir: &str, target_name: &str) {
    let sources = [
        "Lib.hh",
        "MockSpace.cc",
        "MockSpace.hh",
        "Server.cc",
        "Server.hh",
        "Space.hh",
        "CMakeLists.txt",
    ];
    for src in sources.iter() {
        println!("cargo:rerun-if-changed={}/{}", source_dir, src);
    }

    let install_dir = cmake::Config::new(source_dir)
        .build_target("install")
        .build();

    println!(
        "cargo:rustc-link-search=native={}/lib",
        install_dir.display()
    );
    println!("cargo:rustc-link-lib=static={}", target_name);
}

fn generate_bindings(header_path: &str) -> Result<()> {
    // generate bindings
    let bindings = bindgen::Builder::default()
        .header(header_path)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate_comments(false)
        .generate()
        .expect("Unable to generate bindings");

    let out = Path::new("src/bindings.rs");
    let mut file = File::create(&out)?;

    // write to file
    let error_types = [
        "non_camel_case_types",
        "non_snake_case",
        "non_upper_case_globals",
        "dead_code",
        "deref_nullptr",
    ];
    for error_type in error_types.iter() {
        writeln!(file, "#![allow({})]", error_type)?;
    }
    write!(file, "\n{}", bindings.to_string())?;
    Ok(())
}
