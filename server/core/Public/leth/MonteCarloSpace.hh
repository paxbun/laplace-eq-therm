// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_MONTE_CARLO_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_MONTE_CARLO_SPACE_HH

#include <leth/Space.hh>

#include <random>

class MonteCarloSpace : public Space
{
  private:
    enum class ErrorType
    {
        Success,
        InvalidEquation,
    };

  private:
    std::default_random_engine _eng;
    std::vector<bool>          _sanityCheckVisitMap;

  public:
    MonteCarloSpace(uint16_t width, uint16_t height) :
        Space { width, height },
        _eng { std::random_device {}() }
    {}

  protected:
    virtual char const* GetName() noexcept override;

    virtual char const* GetErrorMessage(ErrorCode errorCode) noexcept override;

    virtual ErrorCode RunSimulation(Point const* input, float* output) noexcept override;

  private:
    char const* GetErrorMessageInternal(ErrorType errorType) noexcept;

    ErrorType RunSimulationInternal(Point const* input, float* output) noexcept;

    bool CheckSanity(Point const* input, uint16_t i, uint16_t j) noexcept;

    float DoMonteCarlo(Point const* input, uint16_t i, uint16_t j) noexcept;
};

#endif