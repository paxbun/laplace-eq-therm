// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_SUCCESSIVE_OVER_RELAXATION_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_SUCCESSIVE_OVER_RELAXATION_SPACE_HH

#include "Space.hh"
#include <random>

class SuccessiveOverRelaxationSpace : public Space
{
  public:
    SuccessiveOverRelaxationSpace() = default;

  protected:
    virtual char const* GetName() noexcept override;

    virtual char const* GetErrorMessage(ErrorCode errorCode) noexcept override;

    virtual ErrorCode RunSimulation(Point const* input,
                                    float*       output,
                                    uint16_t     width,
                                    uint16_t     height) noexcept override;
};

#endif