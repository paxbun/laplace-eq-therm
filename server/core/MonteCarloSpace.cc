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

ErrorCode MonteCarloSpace::RunSimulation(Point const* input,
                                         float*       output,
                                         uint16_t     width,
                                         uint16_t     height) noexcept
{
    for (uint16_t i { 0 }; i < height; ++i)
    {
        for (uint16_t j { 0 }; j < width; ++j)
        {
            float sum { 0 };
            for (int repeat { 0 }; repeat < 1000; ++repeat)
                sum += DoMonteCarlo(input, width, height, j, i);
            output[i * width + j] = sum / 1000;
        }
    }

    return 0;
}

float MonteCarloSpace::DoMonteCarlo(Point const* input,
                                    int16_t      width,
                                    int16_t      height,
                                    int16_t      x,
                                    int16_t      y) noexcept
{
    std::uniform_int_distribution<int> dist { 0, 3 };

    while (true)
    {
        size_t idx { (size_t)y * width + (size_t)x };
        if (x < 0 || width <= x || y < 0 || height <= y)
            return 0.0f;

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