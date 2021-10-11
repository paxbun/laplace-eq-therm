// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_LIB_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_LIB_HH

#include "Server.hh"
#include "Space.hh"
#include <cstdint>

using PointType    = Space::PointType;
using ServerHandle = void*;

extern "C"
{
    /// Creates a server instance.
    ///
    /// # Arguments
    ///
    /// * `width`: width of the matrix
    /// * `height`: height of the matrix
    ServerHandle leth_create(uint16_t width, uint16_t height) noexcept;

    /// Returns the number of spaces (algorithm implementers)
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    SpaceIndex leth_get_num_spaces(ServerHandle server) noexcept;

    /// Returns the name (null-terminated) of the space of the given index
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `spaceIdx`: the index of the space
    char const* leth_get_space_name(ServerHandle server, SpaceIndex spaceIdx) noexcept;

    /// Returns the error message corresponding to the given error code.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `spaceIdx`: the index of the space
    /// * `errorCode`: the error code
    char const* leth_get_error_message(ServerHandle server,
                                       SpaceIndex   spaceIdx,
                                       ErrorCode    errorCode) noexcept;

    /// Sets the temperature information of the point.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `x`: the x-coordiate of the point
    /// * `y`: the y-coordiate of the point
    /// * `temp`: current temperature
    /// * `type`: type of the point
    void leth_set(ServerHandle server, uint16_t x, uint16_t y, float temp, PointType type) noexcept;

    /// Gets the simulation result.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    /// * `spaceIdx`: the index of the space
    /// * `temp`: the buffer to store the result. Must be pointing a buffer with size of at least
    ///           `width` * `height` * 4 bytes.
    ErrorCode leth_get(ServerHandle server, SpaceIndex spaceIdx, float* temp) noexcept;

    /// Destroys the given server instance.
    ///
    /// # Arguments
    ///
    /// * `server`: the server instance returned by `leth_create`
    void leth_delete(ServerHandle server) noexcept;
}

#endif