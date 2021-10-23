// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "Lib.hh"
#include "Server.hh"

// Spaces
#include "MonteCarloSpace.hh"
#include "SuccessiveOverRelaxationSpace.hh"

ServerHandle leth_create(uint16_t width, uint16_t height) noexcept
try
{
    return Server::Make<MonteCarloSpace, SuccessiveOverRelaxationSpace>(width, height);
}
catch (...)
{
    return nullptr;
}
