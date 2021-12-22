// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_MATRIX_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_MATRIX_SPACE_HH

#include <leth/Space.hh>

#include <random>
#include <vector>

class MatrixSpace : public Space
{
  private:
    enum class ErrorType
    {
        Success,
        InvalidEquation,
    };

  private:
    std::vector<float>    _A, _x, _b;
    std::vector<Pos>      _i2Pos;
    std::vector<uint32_t> _i2NumNeighbors;
    std::vector<size_t>   _pos2I;

  public:
    MatrixSpace(uint16_t width, uint16_t height);

  protected:
    virtual char const* GetName() noexcept override;

    virtual char const* GetErrorMessage(ErrorCode errorCode) noexcept override final;

    virtual ErrorCode RunSimulation(Point const* input, float* output) noexcept override final;

    /// Solves the equation Ax = b.
    virtual void SolveEquation(std::vector<float> const& A,
                               std::vector<float>&       x,
                               std::vector<float> const& b) noexcept = 0;

  private:
    char const* GetErrorMessageInternal(ErrorType errorType) noexcept;

    ErrorType RunSimulationInternal(Point const* input, float* output) noexcept;

    bool BuildEquation(Point const* input) noexcept;

    void CopyResults(Point const* input, float* output) noexcept;
};

#endif