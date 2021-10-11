use laplace_eq_therm::LocalInfoType;
use laplace_eq_therm_client_common::Client;

// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

pub mod serial;

fn print_usage() {
    eprintln!("Usage: laplace-eq-therm-client <X> <Y> <Type> <Arduino Serial port> <Server URL>");
    eprintln!();
    eprintln!("<Type> is one of the following:");
    eprintln!("* B: Boundary point");
    eprintln!("* I: Internal point (Ground truth)");
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
        eprintln!("* {}", device);
    }
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 6 {
        print_usage();
        return;
    }

    let (x, y): (u16, u16) = match (args[1].parse(), args[2].parse()) {
        (Ok(x), Ok(y)) => (x, y),
        _ => {
            print_usage();
            return;
        }
    };

    let r#type = match args[3].as_str() {
        "B" => LocalInfoType::Boundary,
        "I" => LocalInfoType::GroundTruth,
        _ => {
            print_usage();
            return;
        }
    };

    let mut device = match serial::Arduino::new(&args[4]) {
        Ok(device) => device,
        _ => {
            print_usage();
            return;
        }
    };

    let client = Client::new(args[5].to_owned(), x, y, r#type);

    loop {
        let temperature = device.get_current();
        client.send(temperature);
        println!("Sent: {}", temperature);
    }
}
