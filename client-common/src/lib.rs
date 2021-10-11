// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

//! Implements a common logic (HTTP request) used by the client programs.

use laplace_eq_therm::{LocalInfo, LocalInfoType, SpaceInfo};
use reqwest::blocking::Client as ReqClient;
use serde_json::{from_str, to_string};

/// `Client` sends given temperature information of a point to the server.
pub struct Client {
    client: ReqClient,
    host: String,
    /// x-coordinate of the point
    pub x: u16,
    /// y-coordinate of the point
    pub y: u16,
    /// type of the point
    pub r#type: LocalInfoType,
}

impl Client {
    /// Creates a `Client` instance.
    ///
    /// # Arguments
    ///
    /// * `host`: URL of the server
    /// * `x`: x-coordinate of the point
    /// * `y`: y-coordinate of the point
    /// * `type`: type of the point
    pub fn new(host: String, x: u16, y: u16, r#type: LocalInfoType) -> Self {
        Client {
            client: ReqClient::new(),
            host: host + "/state",
            x,
            y,
            r#type,
        }
    }

    /// Sends the current temperature information to the server and retrieve the
    /// current temperature information of all points.
    pub fn send(&self, temp: f32) -> SpaceInfo {
        let response = self
            .client
            .post(&self.host)
            .body(
                to_string(&LocalInfo {
                    x: self.x,
                    y: self.y,
                    temp,
                    r#type: self.r#type,
                })
                .unwrap(),
            )
            .send()
            .unwrap();

        let body = response.text().unwrap();
        let body: SpaceInfo = from_str(&body).unwrap();

        body
    }
}
