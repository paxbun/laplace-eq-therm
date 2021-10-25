// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include <leth/Lib.hh>
#include <leth/Server.hh>

// Spaces
#include <leth/MonteCarloSpace.hh>
#include <leth/SuccessiveOverRelaxationSpace.hh>

ServerHandle leth_create(uint16_t width, uint16_t height) noexcept
try
{
    return Server::Make<MonteCarloSpace, SuccessiveOverRelaxationSpace>(width, height);
}
catch (...)
{
    return nullptr;
}
