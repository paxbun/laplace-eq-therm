// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "MockSpace.hh"

char const* MockSpace::GetName() noexcept
{
    return "MockSpace";
}

char const* MockSpace::GetErrorMessage(ErrorCode errorCode) noexcept
{
    if (errorCode == 0)
        return "Success";

    return "Unknown error";
}

ErrorCode
MockSpace::RunSimulation(Point const* input, float* output, int16_t width, int16_t height) noexcept
{
    for (int16_t i = 0; i < width * height; ++i) { output[i] = 25.0f; }
    return 0;
}