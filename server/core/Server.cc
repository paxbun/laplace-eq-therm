// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#include "Server.hh"

#include "Lib.hh"
#include "Point.hh"
#include <cstring>


#define CAST_SERVER()                                                                              \
    auto server                                                                                    \
    {                                                                                              \
        (Server*)handle                                                                            \
    }

#pragma region Creation

Server::Server(uint16_t width, uint16_t height, std::vector<std::unique_ptr<Space>>&& spaces) :
    _width { width },
    _height { height },
    _spaces { std::move(spaces) },
    _stopped { false },
    _queueConsumerThread { std::thread { &Server::ConsumeQueue, this } },
    _inputBuffer(GetBufferLength(), Point { PointType::GroundTruth, 0.0f }),
    _outputBufferLocks(_spaces.size()),
    _outputResults(_spaces.size(), 0)
{
    for (size_t i { 0 }, length { _spaces.size() }; i < length; ++i)
    {
        _spaceThreads.push_back(std::thread {
            &Server::CopyBufferAndRunSimulation,
            this,
            i,
            _spaces[i].get(),
        });
        _outputBuffers.push_back(std::vector<float>(GetBufferLength(), 0.0f));
    }
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
    if (errorCode == 0)
    {
        return "Success";
    }
    else if (errorCode == -1)
    {
        return "Invalid space index";
    }
    else if (spaceIdx >= _spaces.size())
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
    std::lock_guard<std::mutex> guard { _requestQueueLock };
    _requestQueue.push_back(SetPointRequest { x, y, temp, type });
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
    std::lock_guard<std::mutex> guard { _inputBufferLock };

    size_t length = GetBufferLength();
    for (size_t i { 0 }; i < length; ++i)
    {
        temp[i] = _inputBuffer[i].temp;
        type[i] = _inputBuffer[i].type;
    }
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
    if (spaceIdx >= _spaces.size())
        return -1;

    std::lock_guard<std::mutex> guard { _outputBufferLocks[spaceIdx] };
    std::memcpy(temp, _outputBuffers[spaceIdx].data(), sizeof(float) * GetBufferLength());

    return _outputResults[spaceIdx];
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

Server::~Server() noexcept
{
    _stopped = true;
    _queueConsumerThread.join();
    for (auto& thread : _spaceThreads) thread.join();
}

#pragma endregion Destruction

void Server::ConsumeQueue() noexcept
{
    while (!_stopped)
    {
        std::lock_guard<std::mutex> inputLockGuard { _inputBufferLock };
        std::lock_guard<std::mutex> queueLockGuard { _requestQueueLock };

        while (!_requestQueue.empty())
        {
            auto& request { _requestQueue.front() };
            if (request.x < _width && request.y < _width)
            {
                auto& point { _inputBuffer[request.x + request.y * _width] };
                point.temp = request.temp;
                point.type = request.type;
            }
            _requestQueue.pop_front();
        }
    }
}

void Server::CopyBufferAndRunSimulation(size_t idx, Space* space) noexcept
{
    while (!_stopped)
    {
        std::vector<Point> input;
        {
            std::lock_guard<std::mutex> guard { _inputBufferLock };
            input = _inputBuffer;
        }

        {
            std::lock_guard<std::mutex> guard { _outputBufferLocks[idx] };
            _outputResults[idx]
                = space->RunSimulation(input.data(), _outputBuffers[idx].data(), _width, _height);
        }
    }
}