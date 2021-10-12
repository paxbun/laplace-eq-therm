// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

//! Implements API endpoints.

use super::server::Server;
use laplace_eq_therm::{LocalInfo, SimulationResults};
use rocket::State;
use rocket_contrib::json::Json;

/// Returns the current simulation results.
///
/// # Arguments
///
/// * `server`: the server state
#[get("/state")]
pub fn get_state(server: State<Server>) -> Json<SimulationResults> {
    Json(server.get_simulation_results())
}

/// Updates the temperature information at the given point and returns the current simulation results.
///
/// # Arguments
///
/// * `state`: new temperature information at the given point
/// * `server`: the server state
#[post("/state", data = "<state>")]
pub fn update_state(state: Json<LocalInfo>, server: State<Server>) -> Json<SimulationResults> {
    server.set(state.x, state.y, state.temp, state.r#type);
    Json(server.get_simulation_results())
}
