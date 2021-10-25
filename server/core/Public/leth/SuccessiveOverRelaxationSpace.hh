// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_SUCCESSIVE_OVER_RELAXATION_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_SUCCESSIVE_OVER_RELAXATION_SPACE_HH

#include <leth/MatrixSpace.hh>

#include <vector>

class SuccessiveOverRelaxationSpace : public MatrixSpace
{
  public:
    using MatrixSpace::MatrixSpace;

  protected:
    virtual char const* GetName() noexcept override final;

    virtual void SolveEquation(std::vector<float> const& A,
                               std::vector<float>&       x,
                               std::vector<float> const& b) noexcept override final;
};

#endif