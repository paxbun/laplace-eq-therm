// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

//! Contains structures used throughout the project.

use serde::{Deserialize, Serialize};

/// Represents the type of a point.
#[derive(Serialize, Deserialize, Copy, Clone, Debug)]
pub enum LocalInfoType {
    /// Represents a boundary point.
    Boundary,
    /// Represents an internal point. The temperature values of the points with this type
    /// is used to be compared with the simulated result.
    GroundTruth,
    /// Represents a point which does not participate in the computation.
    /// The temperature values of the points with this type are ignored.
    OutOfRange,
}

/// Represents temperature information at a point.
#[derive(Serialize, Deserialize, Debug)]
#[serde(rename_all = "camelCase")]
pub struct LocalInfo {
    /// x-coordinate of this point
    pub x: u16,
    /// y-coordinate of this point
    pub y: u16,
    /// current temperature at this point
    pub temp: f32,
    /// type of this temperature information
    pub r#type: LocalInfoType,
}

/// Represents the simulation result of all points at a specific time point.
#[derive(Serialize, Deserialize, Debug)]
#[serde(rename_all = "camelCase")]
pub struct SpaceInfo {
    /// width of the point matrix
    pub width: u16,
    /// height of the point matrix
    pub height: u16,
    /// (Optional) name of the data. Used to distinguish the simulated result and the ground truth.
    pub name: Option<String>,
    /// temperature information corresponding to each point. Each vector must have the same length.
    pub temp: Vec<Vec<f32>>,
}

/// Represents temperature information of all points at a specific time point.
#[derive(Serialize, Deserialize, Debug)]
#[serde(rename_all = "camelCase")]
pub struct GlobalInfo {
    /// temperature
    pub temp: Vec<Vec<f32>>,
    /// point types
    pub r#type: Vec<Vec<LocalInfoType>>,
}

/// Represents a simulation result.
#[derive(Serialize, Deserialize, Debug)]
#[serde(rename_all = "camelCase")]
pub struct SimulationResult {
    /// error code returned during the simulation
    pub error_code: u32,
    /// the simulation result
    pub result: Option<SpaceInfo>,
}

/// Represents a server state.
#[derive(Serialize, Deserialize, Debug)]
#[serde(rename_all = "camelCase")]
pub struct SimulationResults {
    /// temperature information given by the clients
    pub info: GlobalInfo,
    /// simulation results generated by the server
    pub results: Vec<SimulationResult>,
}
