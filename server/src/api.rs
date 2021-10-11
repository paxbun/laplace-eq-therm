// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

//! Implements API endpoints.

use super::server::Server;
use laplace_eq_therm::{LocalInfo, SpaceInfo};
use rocket::State;

#[get("/state")]
pub fn get_state(server: State<Server>) -> SpaceInfo {
    todo!();
}

#[post("/state", data = "<state>")]
pub fn update_state(state: LocalInfo, server: State<Server>) -> SpaceInfo {
    server.set(state.x, state.y, state.temp, state.r#type);
    todo!();
}
