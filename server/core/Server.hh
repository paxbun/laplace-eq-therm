// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_SERVER_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_SERVER_HH

#include "IntegerTypes.hh"
#include "Space.hh"
#include <atomic>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

/// `Server` manages spaces (algorithm implementers).
class Server
{
  private:
    struct SetPointRequest
    {
        uint16_t  x, y;
        float     temp;
        PointType type;
    };

  public:
    template <typename... ArgsT,
              typename std::enable_if<(std::is_base_of<Space, ArgsT>::value && ...), int>::type = 0>
    static Server* Make(uint16_t width, uint16_t height)
    {
        std::vector<std::unique_ptr<Space>> rtn;
        rtn.reserve(sizeof...(ArgsT));
        (rtn.push_back(std::make_unique<ArgsT>(width, height)), ...);

        return new Server { width, height, std::move(rtn) };
    }

  private:
    uint16_t                            _width, _height;
    std::vector<std::unique_ptr<Space>> _spaces;
    std::atomic_bool                    _stopped;

    std::mutex                  _requestQueueLock;
    std::deque<SetPointRequest> _requestQueue;
    std::thread                 _queueConsumerThread;

    std::mutex               _inputBufferLock;
    std::vector<Point>       _inputBuffer;
    std::vector<std::thread> _spaceThreads;

    std::vector<std::mutex>         _outputBufferLocks;
    std::vector<std::vector<float>> _outputBuffers;
    std::vector<ErrorCode>          _outputResults;

  private:
    Server(uint16_t width, uint16_t height, std::vector<std::unique_ptr<Space>>&& spaces);
    Server(Server const&) = delete;
    Server& operator=(Server const&) = delete;

  public:
    ~Server() noexcept;

  public:
    SpaceIndex  GetNumberOfSpaces() noexcept;
    char const* GetSpaceName(SpaceIndex spaceIdx) noexcept;
    char const* GetErrorMessage(SpaceIndex spaceIdx, ErrorCode errorCode) noexcept;
    void        SetPoint(uint16_t x, uint16_t y, float temp, PointType type) noexcept;
    void        GetPoints(float* temp, PointType* type) noexcept;
    ErrorCode   GetSimulationResult(SpaceIndex spaceIdx, float* temp) noexcept;

  private:
    inline size_t GetBufferLength() noexcept
    {
        return ((size_t)_width) * ((size_t)_height);
    }
    void ConsumeQueue() noexcept;
    void CopyBufferAndRunSimulation(size_t idx, Space* space) noexcept;
};

#endif