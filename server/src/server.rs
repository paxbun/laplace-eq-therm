// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

//! Implements `Servers` which exposes C++ part through FFI.

use laplace_eq_therm::LocalInfoType;

use crate::bindings::*;
use std::ffi::c_void;
use std::ffi::CStr;

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
        Server {
            handle: unsafe { leth_create(width, height) },
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

    /// Returns the name (null-terminated) of the space of the given index
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
                    LocalInfoType::Boundary => Space_PointType_Boundary,
                    LocalInfoType::GroundTruth => Space_PointType_GroundTruth,
                    LocalInfoType::OutOfRange => Space_PointType_OutOfRange,
                },
            )
        }
    }

    /// Gets the simulation result.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `spaceIdx`: the index of the space
    /// * `temp`: the buffer to store the result. Must be pointing a buffer with size of at least
    ///           `width` * `height` * 4 bytes.
    pub fn get(&self, space_idx: u32, temp: &mut [f32]) -> u32 {
        assert!(temp.len() >= self.width as usize * self.height as usize);
        unsafe { leth_get(self.handle, space_idx, temp.as_mut_ptr()) }
    }
}

impl Drop for Server {
    fn drop(&mut self) {
        unsafe { leth_delete(self.handle) };
        self.handle = std::ptr::null_mut();
    }
}
