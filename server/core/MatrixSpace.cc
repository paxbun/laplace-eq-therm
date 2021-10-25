// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "MatrixSpace.hh"

#include <limits>
#include <random>

char const* MatrixSpace::GetName() noexcept
{
    return "MatrixSpace";
}

char const* MatrixSpace::GetErrorMessage(ErrorCode errorCode) noexcept
{
    return GetErrorMessageInternal(static_cast<ErrorType>(errorCode));
}

char const* MatrixSpace::GetErrorMessageInternal(ErrorType errorType) noexcept
{
    switch (errorType)
    {
    case ErrorType::Success: return "Success";
    case ErrorType::BufferAllocationFailed: return "Failed to allocate memory for matrices";
    case ErrorType::InvalidEquation: return "Insufficient boundary condition";
    }

    return "Unknown error";
}

ErrorCode MatrixSpace::RunSimulation(Point const* input,
                                     float*       output,
                                     uint16_t     width,
                                     uint16_t     height) noexcept
{
    return static_cast<ErrorCode>(RunSimulationInternal(input, output, width, height));
}

MatrixSpace::ErrorType MatrixSpace::RunSimulationInternal(Point const* input,
                                                          float*       output,
                                                          uint16_t     width,
                                                          uint16_t     height) noexcept
{
    if (!BuildBuffer(width, height))
        return ErrorType::BufferAllocationFailed;

    if (!BuildEquation(input, width, height))
        return ErrorType::InvalidEquation;

    CopyNonBoundaryPoints(input, output, width, height);
    SolveEquation(output, width, height);

    return ErrorType::Success;
}

bool MatrixSpace::BuildBuffer(uint16_t width, uint16_t height) noexcept
try
{
    _A.reserve(static_cast<size_t>(width) * width * height * height);
    _A.resize(0);
    _x.reserve(static_cast<size_t>(width) * height);
    _x.resize(0);
    _b.reserve(static_cast<size_t>(width) * height);
    _b.resize(0);
    _pos.reserve(static_cast<size_t>(width) * height);
    _pos.resize(0);
    return true;
}
catch (...)
{
    return false;
}

bool MatrixSpace::BuildEquation(Point const* input, uint16_t width, uint16_t height) noexcept
{
    return false;
}

void MatrixSpace::CopyNonBoundaryPoints(Point const* input,
                                        float*       output,
                                        uint16_t     width,
                                        uint16_t     height) noexcept
{
    for (uint16_t i { 0 }; i < height; ++i)
    {
        for (uint16_t j { 0 }; j < width; ++j)
        {
            size_t idx { static_cast<size_t>(i) * width + j };
            switch (input[idx].type)
            {
            case PointType::Boundary: output[idx] = 0.0f; break;
            case PointType::GroundTruth: output[idx] = input[idx].temp; break;
            }
        }
    }
}

void MatrixSpace::SolveEquation(float* output, uint16_t width, uint16_t height) noexcept
{
    //
}