// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

//! Implements API endpoints.

use super::server::Server;
use laplace_eq_therm::{LocalInfo, SimulationResult, SimulationResults};
use rocket::State;
use rocket_contrib::json::Json;

fn get_simulation_results(server: &Server) -> SimulationResults {
    (0..server.get_num_spaces())
        .map(|i| match server.get(i) {
            Ok(result) => SimulationResult {
                error_code: 0,
                result: Some(result),
            },
            Err(error_code) => SimulationResult {
                error_code,
                result: None,
            },
        })
        .collect()
}

/// Returns the current simulation results.
///
/// # Arguments
///
/// * `server`: the server state
#[get("/state")]
pub fn get_state(server: State<Server>) -> Json<SimulationResults> {
    Json(get_simulation_results(&server))
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
    Json(get_simulation_results(&server))
}
