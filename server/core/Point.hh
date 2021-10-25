// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_POINT_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_POINT_HH

#include <cstdint>

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

struct Pos
{
    uint16_t x, y;
};

#endif