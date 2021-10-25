// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "MonteCarloSpace.hh"

#include <limits>
#include <random>

char const* MonteCarloSpace::GetName() noexcept
{
    return "Monte Carlo";
}

char const* MonteCarloSpace::GetErrorMessage(ErrorCode errorCode) noexcept
{
    if (errorCode == 0)
        return "Success";

    return "Unknown error";
}

ErrorCode MonteCarloSpace::RunSimulation(Point const* input, float* output) noexcept
{
    for (uint16_t i { 0 }, iEnd { height() }; i < iEnd; ++i)
    {
        for (uint16_t j { 0 }, jEnd { width() }; j < jEnd; ++j)
        {
            float sum { 0 };
            for (int repeat { 0 }; repeat < 1000; ++repeat) sum += DoMonteCarlo(input, i, j);
            output[GetIndex(i, j)] = sum / 1000;
        }
    }

    return 0;
}

float MonteCarloSpace::DoMonteCarlo(Point const* input, uint16_t i, uint16_t j) noexcept
{
    std::uniform_int_distribution<int> dist { 0, 3 };

    int16_t x { static_cast<int16_t>(j) }, y { static_cast<int16_t>(i) };
    while (true)
    {
        if (!Inside(y, x))
            return 0.0f;

        auto idx { GetIndex(y, x) };
        if (input[idx].type == PointType::Boundary)
            return input[idx].temp;

        if (input[idx].type == PointType::OutOfRange)
            return 0.0f;

        switch (dist(_eng))
        {
        case 0: --y; break;
        case 1: ++x; break;
        case 2: ++y; break;
        case 3: --x; break;
        }
    }
}