// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#![allow(non_upper_case_globals)]

//! Implements `Servers` which exposes C++ part through FFI.

use crate::bindings::*;
use laplace_eq_therm::{GlobalInfo, LocalInfoType, SimulationResult, SimulationResults};
use std::ffi::{c_void, CStr};
use std::ptr::null_mut;

/// `Server` implements simulation algorithms and returns the results.
pub struct Server {
    handle: *mut c_void,
    width: u16,
    height: u16,
}

// Thread-safety is implemented by the C++ part
unsafe impl Send for Server {}

// Thread-safety is implemented by the C++ part
unsafe impl Sync for Server {}

impl Server {
    /// Creates a server instance.
    ///
    /// # Arguments
    ///
    /// * `width`: width of the matrix
    /// * `height`: height of the matrix
    pub fn new(width: u16, height: u16) -> Self {
        let handle = unsafe { leth_create(width, height) };
        assert!(handle != std::ptr::null_mut());
        Server {
            handle,
            width,
            height,
        }
    }

    /// Returns the size of the matrix
    pub fn size(&self) -> (u16, u16) {
        (self.width, self.height)
    }

    /// Returns the number of spaces (algorithm implementers)
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    pub fn get_num_spaces(&self) -> u32 {
        (unsafe { leth_get_num_spaces(self.handle) }) as u32
    }

    /// Returns the name of the space of the given index
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `spaceIdx`: the index of the space
    pub fn get_space_name(&self, space_idx: u32) -> &'static str {
        let str = unsafe { CStr::from_ptr(leth_get_space_name(self.handle, space_idx)) };
        str.to_str().unwrap()
    }

    /// Returns the error message corresponding to the given error code.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `spaceIdx`: the index of the space
    /// * `errorCode`: the error code
    pub fn get_error_message(&self, space_idx: u32, error_code: u32) -> &'static str {
        let str =
            unsafe { CStr::from_ptr(leth_get_error_message(self.handle, space_idx, error_code)) };
        str.to_str().unwrap()
    }

    /// Sets the temperature information of the point.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `x`: the x-coordiate of the point
    /// * `y`: the y-coordiate of the point
    /// * `temp`: current temperature
    /// * `ty`: type of the point
    pub fn set(&self, x: u16, y: u16, temp: f32, ty: LocalInfoType) {
        unsafe {
            leth_set(
                self.handle,
                x,
                y,
                temp,
                match ty {
                    LocalInfoType::Boundary => PointType_Boundary,
                    LocalInfoType::GroundTruth => PointType_GroundTruth,
                    LocalInfoType::OutOfRange => PointType_OutOfRange,
                },
            )
        }
    }

    /// Returns the current temperature information of all points in a raw form.
    ///
    /// # Arguments
    ///
    /// * `temp`: the buffer to store temperature. Must be pointing a buffer with size of at least
    ///           `width` * `height` * 4 bytes.
    /// * `ty`: the buffer to store point types. Must be pointing a buffer with size of at least
    ///         `width` * `height` bytes.
    pub fn get_raw(&self, temp: &mut [f32], ty: &mut [u8]) {
        unsafe { leth_get(self.handle, temp.as_mut_ptr(), ty.as_mut_ptr()) }
    }

    /// Returns the current temperature information of all points.
    pub fn get(&self) -> GlobalInfo {
        let buff_len = self.width as usize * self.height as usize;

        let mut temp = Vec::new();
        temp.resize(buff_len, 0.0f32);

        let mut ty = Vec::new();
        ty.resize(buff_len, 0);

        self.get_raw(&mut temp, &mut ty);

        GlobalInfo {
            temp: temp
                .chunks(self.width as usize)
                .map(|s| Vec::from(s))
                .collect(),
            r#type: ty
                .chunks(self.width as usize)
                .map(|s| {
                    s.iter()
                        .map(|&i| match i {
                            PointType_Boundary => LocalInfoType::Boundary,
                            PointType_GroundTruth => LocalInfoType::GroundTruth,
                            _ => LocalInfoType::OutOfRange,
                        })
                        .collect()
                })
                .collect(),
        }
    }

    /// Gets the simulation result in a raw form. Returns an error code. Error code is nonzero if the simulation failed.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `spaceIdx`: the index of the space
    /// * `temp`: the buffer to store the result. Must be pointing a buffer with size of at least
    ///           `width` * `height` * 4 bytes.
    pub fn get_result_raw(&self, space_idx: u32, temp: &mut [f32]) -> u32 {
        assert!(temp.len() >= self.width as usize * self.height as usize);
        unsafe { leth_get_res(self.handle, space_idx, temp.as_mut_ptr()) }
    }

    /// Gets the simulation result. Returns a `SpaceInfo` instance if succeeded; an error code otherwise.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `spaceIdx`: the index of the space
    pub fn get_result(&self, space_idx: u32) -> SimulationResult {
        let mut buff = Vec::new();
        buff.resize(self.width as usize * self.height as usize, 0.0f32);

        let name = self.get_space_name(space_idx);
        let error_code = self.get_result_raw(space_idx, &mut buff);
        let error_message = self.get_error_message(space_idx, error_code);
        SimulationResult {
            name: String::from(name),
            error_code,
            error_message: String::from(error_message),
            temp: if error_code != 0 {
                None
            } else {
                Some(
                    buff.chunks(self.width as usize)
                        .map(|s| Vec::from(s))
                        .collect(),
                )
            },
        }
    }

    /// Returns server state in a form of `SimulationResults`.
    pub fn get_simulation_results(&self) -> SimulationResults {
        SimulationResults {
            width: self.width,
            height: self.height,
            info: self.get(),
            results: (0..self.get_num_spaces())
                .map(|i| self.get_result(i))
                .collect(),
        }
    }
}

impl Drop for Server {
    fn drop(&mut self) {
        unsafe { leth_delete(self.handle) };
        self.handle = null_mut();
    }
}
