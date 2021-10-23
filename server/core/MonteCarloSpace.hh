// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_MONTE_CARLO_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_MONTE_CARLO_SPACE_HH

#include "Space.hh"
#include <random>

class MonteCarloSpace : public Space
{
  private:
    std::default_random_engine _eng;

  public:
    MonteCarloSpace() : _eng { std::random_device {}() } {}

  protected:
    virtual char const* GetName() noexcept override;

    virtual char const* GetErrorMessage(ErrorCode errorCode) noexcept override;

    virtual ErrorCode RunSimulation(Point const* input,
                                    float*       output,
                                    uint16_t     width,
                                    uint16_t     height) noexcept override;

  private:
    float
    DoMonteCarlo(Point const* input, int16_t width, int16_t height, int16_t x, int16_t y) noexcept;
};

#endif