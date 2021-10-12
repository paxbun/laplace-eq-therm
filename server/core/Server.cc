// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "Server.hh"

#include "Lib.hh"
#include "MockSpace.hh"

#define CAST_SERVER()                                                                              \
    auto server                                                                                    \
    {                                                                                              \
        (Server*)handle                                                                            \
    }

using PointType = Space::PointType;

#pragma region Creation

Server::Server(uint16_t width, uint16_t height, std::vector<std::unique_ptr<Space>>&& spaces) :
    _width { width },
    _height { height },
    _spaces { std::move(spaces) }
{}

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

#pragma endregion Creation

#pragma region GetNumberOfSpaces

SpaceIndex Server::GetNumberOfSpaces() noexcept
{
    return (SpaceIndex)_spaces.size();
}

SpaceIndex leth_get_num_spaces(ServerHandle handle) noexcept
{
    CAST_SERVER();
    return server->GetNumberOfSpaces();
}

#pragma endregion GetNumberOfSpaces

#pragma region GetSpaceName

char const* Server::GetSpaceName(SpaceIndex spaceIdx) noexcept
{
    if (spaceIdx >= _spaces.size())
        return "Invalid space";
    else
        return _spaces[spaceIdx]->GetName();
}

char const* leth_get_space_name(ServerHandle handle, SpaceIndex spaceIdx) noexcept
{
    CAST_SERVER();
    return server->GetSpaceName(spaceIdx);
}

#pragma endregion GetSpaceName

#pragma region GetErrorMessage

char const* Server::GetErrorMessage(SpaceIndex spaceIdx, ErrorCode errorCode) noexcept
{
    if (spaceIdx >= _spaces.size())
        return "Unknown error";
    else
        return _spaces[spaceIdx]->GetErrorMessage(errorCode);
}

char const* leth_get_error_message(ServerHandle handle,
                                   SpaceIndex   spaceIdx,
                                   ErrorCode    errorCode) noexcept
{
    CAST_SERVER();
    return server->GetErrorMessage(spaceIdx, errorCode);
}

#pragma endregion GetErrorMessage

#pragma region SetPoint

void Server::SetPoint(uint16_t x, uint16_t y, float temp, PointType type) noexcept
{
    // TODO
}

void leth_set(ServerHandle handle, uint16_t x, uint16_t y, float temp, PointType type) noexcept
{
    CAST_SERVER();
    server->SetPoint(x, y, temp, type);
}

#pragma endregion SetPoint

#pragma region GetPoints

void Server::GetPoints(float* temp, PointType* type) noexcept
{
    // TODO
}

void leth_get(ServerHandle handle, float* temp, PointType* type) noexcept
{
    CAST_SERVER();
    server->GetPoints(temp, type);
}

#pragma endregion GetPoints

#pragma region GetSimulationResult

ErrorCode Server::GetSimulationResult(SpaceIndex spaceIdx, float* temp) noexcept
{
    // TODO
    return 2;
}

ErrorCode leth_get_res(ServerHandle handle, SpaceIndex spaceIdx, float* temp) noexcept
{
    CAST_SERVER();
    return server->GetSimulationResult(spaceIdx, temp);
}

#pragma endregion GetSimulationResult

#pragma region Destruction

void leth_delete(ServerHandle handle) noexcept
{
    CAST_SERVER();
    delete server;
}

#pragma endregion Destruction