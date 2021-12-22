// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <leth/MonteCarloSpace.hh>

#include <limits>
#include <random>
#include <vector>

char const* MonteCarloSpace::GetName() noexcept
{
    return "Monte Carlo";
}

char const* MonteCarloSpace::GetErrorMessage(ErrorCode errorCode) noexcept
{
    return GetErrorMessageInternal(static_cast<ErrorType>(errorCode));
}

char const* MonteCarloSpace::GetErrorMessageInternal(ErrorType errorCode) noexcept
{
    switch (errorCode)
    {
    case ErrorType::Success: return "Success";
    case ErrorType::InvalidEquation: return "Insufficient boundary condition";
    }

    return "Unknown error";
}

ErrorCode MonteCarloSpace::RunSimulation(Point const* input, float* output) noexcept
{
    return static_cast<ErrorCode>(RunSimulationInternal(input, output));
}

MonteCarloSpace::ErrorType MonteCarloSpace::RunSimulationInternal(Point const* input,
                                                                  float*       output) noexcept
{
    for (uint16_t i { 0 }, iEnd { height() }; i < iEnd; ++i)
    {
        for (uint16_t j { 0 }, jEnd { width() }; j < jEnd; ++j)
        {
            if (!CheckSanity(input, i, j))
                return ErrorType::InvalidEquation;
        }
    }

    for (uint16_t i { 0 }, iEnd { height() }; i < iEnd; ++i)
    {
        for (uint16_t j { 0 }, jEnd { width() }; j < jEnd; ++j)
        {
            float sum { 0 };
            for (int repeat { 0 }; repeat < 1000; ++repeat) sum += DoMonteCarlo(input, i, j);
            output[GetIndex(i, j)] = sum / 1000;
        }
    }

    return ErrorType::Success;
}

bool MonteCarloSpace::CheckSanity(Point const* input, uint16_t i, uint16_t j) noexcept
{

    int16_t x { static_cast<int16_t>(j) }, y { static_cast<int16_t>(i) };
    if (!Inside(y, x))
        return true;

    auto idx { GetIndex(y, x) };
    if (input[idx].type != PointType::GroundTruth)
        return true;

    constexpr int16_t offsets[4][2] {
        { -1, 0 },
        { 0, 1 },
        { 1, 0 },
        { 0, -1 },
    };

    _sanityCheckVisitMap.resize(static_cast<size_t>(width()) * height());
    std::fill(_sanityCheckVisitMap.begin(), _sanityCheckVisitMap.end(), false);
    _sanityCheckVisitMap[idx] = true;

    std::vector<Pos> stack;
    stack.push_back(Pos { static_cast<uint16_t>(x), static_cast<uint16_t>(y) });
    while (!stack.empty())
    {
        Pos back { stack.back() };
        stack.pop_back();

        if (!Inside(back.y, back.x))
            continue;

        auto idx { GetIndex(back.y, back.x) };
        if (input[idx].type == PointType::Boundary)
            return true;

        if (input[idx].type == PointType::OutOfRange)
            continue;

        for (auto& offset : offsets)
        {
            if (!Inside(back.y + offset[0], back.x + offset[1]))
                continue;

            auto newIdx { GetIndex(back.y + offset[0], back.x + offset[1]) };
            if (_sanityCheckVisitMap[newIdx])
                continue;

            _sanityCheckVisitMap[newIdx] = true;
            stack.push_back(Pos {
                static_cast<uint16_t>(back.x + offset[1]),
                static_cast<uint16_t>(back.y + offset[0]),
            });
        }
    }

    return false;
}

float MonteCarloSpace::DoMonteCarlo(Point const* input, uint16_t i, uint16_t j) noexcept
{
    std::uniform_int_distribution<int> dist { 0, 3 };

    int16_t beforeX, beforeY;
    int16_t x { static_cast<int16_t>(j) }, y { static_cast<int16_t>(i) };

    if (!Inside(y, x))
        return 0.0f;

    auto idx { GetIndex(y, x) };
    if (input[idx].type == PointType::OutOfRange)
        return 0.0f;

    while (true)
    {
        if (!Inside(y, x))
        {
            x = beforeX;
            y = beforeY;
            continue;
        }

        auto idx { GetIndex(y, x) };
        if (input[idx].type == PointType::Boundary)
            return input[idx].temp;

        if (input[idx].type == PointType::OutOfRange)
        {
            x = beforeX;
            y = beforeY;
            continue;
        }

        beforeX = x;
        beforeY = y;

        switch (dist(_eng))
        {
        case 0: --y; break;
        case 1: ++x; break;
        case 2: ++y; break;
        case 3: --x; break;
        }
    }
}