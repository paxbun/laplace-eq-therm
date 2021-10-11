// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_SERVER_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_SERVER_HH

#include "Space.hh"
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

using SpaceIndex = uint32_t;

/// `Server` manages spaces (algorithm implementers).
class Server
{
  public:
    template <typename... ArgsT,
              std::enable_if_t<(std::is_base_of_v<Space, ArgsT> && ...), int> = 0>
    Server* Make(uint16_t width, uint16_t height)
    {
        return new Server
        {
            width, height,
            {
                std::make_unique<ArgsT>()...
            }
        }
    }

  private:
    uint16_t                            _width, _height;
    std::vector<std::unique_ptr<Space>> _spaces;

  private:
    Server(uint16_t width, uint16_t height, std::vector<std::unique_ptr<Space>>&& spaces);
    Server(Server const&) = delete;
    Server& operator=(Server const&) = delete;

  public:
    SpaceIndex  GetNumberOfSpaces() noexcept;
    char const* GetSpaceName(SpaceIndex spaceIdx) noexcept;
    char const* GetErrorMessage(SpaceIndex spaceIdx, ErrorCode errorCode) noexcept;
    void        SetPoint(uint16_t x, uint16_t y, float temp, Space::PointType type) noexcept;
    ErrorCode   GetSimulationResult(SpaceIndex spaceIdx, float* temp) noexcept;
};

#endif