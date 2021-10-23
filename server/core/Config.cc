// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "Lib.hh"
#include "MockSpace.hh"
#include "Server.hh"


ServerHandle leth_create(uint16_t width, uint16_t height) noexcept
try
{
    // TODO
    return Server::Make<MockSpace>(width, height);
}
catch (...)
{
    return nullptr;
}
