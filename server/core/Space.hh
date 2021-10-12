// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_SPACE_HH

#include <cstdint>

using ErrorCode = uint32_t;

/// Represents an algorithm implementer.
class Space
{
    friend class Server;

  public:
    /// Represents the type of a point. (See src/lib.rs)
    enum class PointType : uint8_t
    {
        /// Represents a boundary point.
        Boundary = 0,
        /// Represents an internal point. The temperature values of the points with this type
        /// is used to be compared with the simulated result.
        GroundTruth = 1,
        /// Represents a point which does not participate in the computation.
        /// The temperature values of the points with this type are ignored.
        OutOfRange = 2,
    };

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
    RunSimulation(Point const* input, float* output, uint16_t width, uint16_t height) noexcept = 0;

  public:
    virtual ~Space() {}
};

#endif