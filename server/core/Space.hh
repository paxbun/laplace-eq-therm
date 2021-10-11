// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_SPACE_HH

#include "Lib.hh"
#include <cstdint>
#include <span>

/// Represents an algorithm implementer.
class Space
{
  public:
    struct Point
    {
        PointType type;
        float     temp;
    };

  protected:
    /// Returns the name of this space. Must be a static string.
    virtual char const* GetName() noexcept = 0;

    /// Returns the error message corresponding to the given error code. Must be a static string.
    virtual char const* GetErrorMessage(ErrorCode errorCode) noexcept = 0;

    // Runs the simulation.
    virtual ErrorCode
    RunSimulation(Point const* input, float* output, int16_t width, int16_t height) noexcept = 0;
};

#endif