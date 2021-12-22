// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <leth/MatrixSpace.hh>

#include <limits>
#include <random>

MatrixSpace::MatrixSpace(uint16_t width, uint16_t height) :
    Space { width, height },
    _pos2I(static_cast<size_t>(width) * height, std::numeric_limits<size_t>::max())
{
    _A.reserve(static_cast<size_t>(width) * width * height * height);
    _x.reserve(static_cast<size_t>(width) * height);
    _b.reserve(static_cast<size_t>(width) * height);
    _i2Pos.reserve(static_cast<size_t>(width) * height);
}

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
    case ErrorType::InvalidEquation: return "Insufficient boundary condition";
    }

    return "Unknown error";
}

ErrorCode MatrixSpace::RunSimulation(Point const* input, float* output) noexcept
{
    return static_cast<ErrorCode>(RunSimulationInternal(input, output));
}

MatrixSpace::ErrorType MatrixSpace::RunSimulationInternal(Point const* input,
                                                          float*       output) noexcept
{
    if (!BuildEquation(input))
        return ErrorType::InvalidEquation;

    SolveEquation(_A, _x, _b);
    CopyResults(input, output);

    return ErrorType::Success;
}

bool MatrixSpace::BuildEquation(Point const* input) noexcept
{
    _A.clear();
    _x.clear();
    _b.clear();
    _i2Pos.clear();
    _i2NumNeighboringWalls.clear();

    constexpr int16_t offsets[4][2] {
        { -1, 0 },
        { 0, 1 },
        { 1, 0 },
        { 0, -1 },
    };

    for (uint16_t i { 0 }, iEnd { height() }; i < iEnd; ++i)
    {
        for (uint16_t j { 0 }, jEnd { width() }; j < jEnd; ++j)
        {
            size_t const idx { GetIndex(i, j) };
            if (input[idx].type == PointType::GroundTruth)
            {
                uint32_t numNeighboringWalls { 0 };
                for (auto& offset : offsets)
                {
                    if (!Inside(i + offset[0], j + offset[1]))
                    {
                        ++numNeighboringWalls;
                        continue;
                    }

                    if (input[GetIndex(i + offset[0], j + offset[1])].type == PointType::OutOfRange)
                    {
                        ++numNeighboringWalls;
                        continue;
                    }
                }

                _pos2I[idx] = _i2Pos.size();
                _i2NumNeighboringWalls.push_back(numNeighboringWalls);
                _i2Pos.push_back(Pos { static_cast<uint16_t>(j), static_cast<uint16_t>(i) });
            }
            else
                _pos2I[idx] = std::numeric_limits<size_t>::max();
        }
    }

    size_t const numVars { _i2Pos.size() };
    _A.resize(numVars * numVars, 0.0f);
    _x.resize(numVars, 0.0f);
    _b.resize(numVars, 0.0f);

    for (size_t i { 0 }, iEnd { _i2Pos.size() }; i < iEnd; ++i)
    {
        _A[i * numVars + i] = -4 + static_cast<float>(_i2NumNeighboringWalls[i]);

        size_t const idx { GetIndex(_i2Pos[i].y, _i2Pos[i].x) };
        for (auto& offset : offsets)
        {
            if (!Inside(_i2Pos[i].y + offset[0], _i2Pos[i].x + offset[1]))
                continue;

            auto const offsetAppliedIdx {
                GetIndex(_i2Pos[i].y + offset[0], _i2Pos[i].x + offset[1]),
            };
            switch (input[offsetAppliedIdx].type)
            {
            case PointType::Boundary:
            {
                _b[i] -= input[offsetAppliedIdx].temp;
                break;
            }
            case PointType::GroundTruth:
            {
                size_t const offsetAppliedI { _pos2I[offsetAppliedIdx] };
                _A[i * numVars + offsetAppliedI] += 1;
                break;
            }
            }
        }
    }

    return true;
}

void MatrixSpace::CopyResults(Point const* input, float* output) noexcept
{
    for (size_t i { 0 }, iEnd { _i2Pos.size() }; i < iEnd; ++i)
    {
        size_t idx { GetIndex(_i2Pos[i].y, _i2Pos[i].x) };
        output[idx] = _x[i];
    }

    for (uint16_t i { 0 }, iEnd { height() }; i < iEnd; ++i)
    {
        for (uint16_t j { 0 }, jEnd { width() }; j < jEnd; ++j)
        {
            size_t idx { GetIndex(i, j) };
            if (input[idx].type == PointType::Boundary)
                output[idx] = input[idx].temp;
        }
    }
}