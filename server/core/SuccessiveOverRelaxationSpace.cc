// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "SuccessiveOverRelaxationSpace.hh"

#include <limits>
#include <random>

char const* SuccessiveOverRelaxationSpace::GetName() noexcept
{
    return "SOR";
}

char const* SuccessiveOverRelaxationSpace::GetErrorMessage(ErrorCode errorCode) noexcept
{
    if (errorCode == 0)
        return "Success";

    return "Unknown error";
}

ErrorCode SuccessiveOverRelaxationSpace::RunSimulation(Point const* input,
                                                       float*       output,
                                                       uint16_t     width,
                                                       uint16_t     height) noexcept
{
    for (uint16_t i { 0 }; i < height; ++i)
    {
        for (uint16_t j { 0 }; j < width; ++j)
        {
            // TODO
            output[i * width + j] = 12.0f;
        }
    }

    return 0;
}
