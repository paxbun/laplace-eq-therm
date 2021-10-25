// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "SuccessiveOverRelaxationSpace.hh"

char const* SuccessiveOverRelaxationSpace::GetName() noexcept
{
    return "SOR";
}

void SuccessiveOverRelaxationSpace::SolveEquation(std::vector<float> const& A,
                                                  std::vector<float>&       x,
                                                  std::vector<float> const& b) noexcept
{
    constexpr float omega { 1.12 };

    size_t const numVars { x.size() };
    for (size_t iter { 0 }; iter < 100; ++iter)
    {
        for (size_t i { 0 }; i < numVars; ++i)
        {
            float before { x[i] };

            x[i] = b[i];

            for (size_t j { 0 }; j < i; ++j) x[i] -= (A[i * numVars + j] * x[j]);

            for (size_t j { i + 1 }; j < numVars; ++j) x[i] -= (A[i * numVars + j] * x[j]);

            x[i] *= omega;
            x[i] /= A[i * numVars + i];
            x[i] += (1 - omega) * before;
        }
    }
}