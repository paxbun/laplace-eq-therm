// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_FINITE_ELEMENT_METHOD_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_FINITE_ELEMENT_METHOD_SPACE_HH

#include <leth/Space.hh>

class FiniteElementMethodSpace : public Space
{
  public:
    FiniteElementMethodSpace(uint16_t width, uint16_t height);

  protected:
    virtual char const* GetName() noexcept override;

    virtual char const* GetErrorMessage(ErrorCode errorCode) noexcept override final;

    virtual ErrorCode RunSimulation(Point const* input, float* output) noexcept override final;
};

#endif