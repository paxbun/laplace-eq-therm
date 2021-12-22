// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <leth/FiniteElementMethodSpace.hh>

FiniteElementMethodSpace::FiniteElementMethodSpace(uint16_t width, uint16_t height) :
    Space { width, height }
{}

char const* FiniteElementMethodSpace::GetName() noexcept
{
    return "FiniteElementMethodSpace";
}

char const* FiniteElementMethodSpace::GetErrorMessage(ErrorCode errorCode) noexcept
{
    if (errorCode == 0)
        return "Success";

    return "UnknownError";
}

ErrorCode FiniteElementMethodSpace::RunSimulation(Point const* input, float* output) noexcept
{
    return 0;
}
