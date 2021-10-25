// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_SPACE_HH

#include "IntegerTypes.hh"
#include "Point.hh"
#include <cstdint>

/// Represents an algorithm implementer.
class Space
{
    friend class Server;

  private:
    uint16_t _width, _height;

  protected:
    /// Returns the width of the input and the output matrix
    uint16_t width() const noexcept
    {
        return _width;
    }

    /// Returns the height of the input and the output matrix
    uint16_t height() const noexcept
    {
        return _height;
    }

  public:
    Space(uint16_t width, uint16_t height) : _width { width }, _height { height } {}

  protected:
    /// Returns the name of this space. Must be a static string.
    virtual char const* GetName() noexcept = 0;

    /// Returns the error message corresponding to the given error code. Must be a static string.
    virtual char const* GetErrorMessage(ErrorCode errorCode) noexcept = 0;

    // Runs the simulation.
    virtual ErrorCode RunSimulation(Point const* input, float* output) noexcept = 0;

    /// Returns the index of the element of the input and output buffer corresponding to (i, j).
    size_t GetIndex(uint16_t i, uint16_t j) const noexcept
    {
        return static_cast<size_t>(i) * _width + j;
    }

    /// Returns the index of the element of the input and output buffer corresponding to (i, j).
    ptrdiff_t GetIndex(int32_t i, int32_t j) const noexcept
    {
        return static_cast<ptrdiff_t>(i) * _width + j;
    }

    /// Returns whether the point (i, j) is inside the matrix.
    bool Inside(uint16_t i, uint16_t j) const noexcept
    {
        return i < _width && j < _height;
    }

    /// Returns whether the point (i, j) is inside the matrix.
    bool Inside(int32_t i, int32_t j) const noexcept
    {
        return 0 <= i && i < _width && 0 <= j && j < _height;
    }

  public:
    virtual ~Space() {}
};

#endif