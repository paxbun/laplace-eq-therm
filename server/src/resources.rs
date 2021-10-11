// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

//! Implements static resource endpoint handling.

use rocket::http::{ContentType, Status};
use rocket::Response;
use std::io::Cursor;

macro_rules! include_static_resources {
    ($($content_type:expr, $filename:literal;)+) => {
        static STATIC_RESOURCES: &[(ContentType, &str, &[u8])] = &[
            $(($content_type, $filename, include_bytes!(concat!("../resources/", $filename)))),+
        ];
    };
}

include_static_resources! [
    ContentType::JavaScript,    "index.js"      ;
    ContentType::CSS,           "index.css"     ;
    ContentType::HTML,          "index.html"    ;
];

/// Returns `index.html`.
#[get("/")]
pub fn get_index() -> rocket::response::Response<'static> {
    get_static_resource(String::from("index.html"))
}

/// Returns the static resource with the given filename if found. Returns 404 otherwise.
///
/// # Arguments
///
/// * `filename`: name of the resource
#[get("/<filename>")]
pub fn get_static_resource(filename: String) -> rocket::response::Response<'static> {
    for (content_type, name, resource) in STATIC_RESOURCES {
        if &filename == name {
            return Response::build()
                .header(content_type.clone())
                .status(Status::Ok)
                .sized_body(Cursor::new(resource))
                .finalize();
        }
    }

    return Response::build().status(Status::NotFound).finalize();
}
