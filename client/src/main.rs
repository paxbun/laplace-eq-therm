// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

pub mod serial;

fn print_usage() {
    eprintln!("Usage: laplace-eq-therm-client <Arduino Serial port> <Server URL>");
    eprintln!();
    let devices = serial::Arduino::get_available_ports();
    eprintln!(
        "{} Arduino {} found",
        devices.len(),
        if devices.len() == 1 {
            "device"
        } else {
            "devices"
        }
    );
    for device in devices {
        eprintln!(" * {}", device);
    }
}

fn main() {
    print_usage();
}
