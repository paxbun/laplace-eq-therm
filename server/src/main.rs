// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#![feature(proc_macro_hygiene, decl_macro)]
#[macro_use]
extern crate rocket;

pub mod api;
pub mod bindings;
pub mod resources;
pub mod server;

fn print_usage() {
    eprintln!("Usage: laplace-eq-therm-server <width of the matrix> <height of the matrix>");
}

#[rocket::main]
async fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 3 {
        print_usage();
        return;
    }

    let (width, height): (u16, u16) = match (args[1].parse(), args[2].parse()) {
        (Ok(width), Ok(height)) => (width, height),
        _ => {
            print_usage();
            return;
        }
    };

    rocket::build()
        .manage(server::Server::new(width, height))
        .mount(
            "/",
            routes![
                api::get_state,
                api::update_state,
                resources::get_index,
                resources::get_static_resource
            ],
        )
        .launch()
        .await
        .unwrap();
}
