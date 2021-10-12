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

ErrorCode MockSpace::RunSimulation(Point const* input,
                                   float*       output,
                                   uint16_t     width,
                                   uint16_t     height) noexcept
{
    for (uint16_t i = 0; i < height; ++i)
    {
        for (uint16_t j = 0; j < width; ++j)
        {
            
        }
    }
}