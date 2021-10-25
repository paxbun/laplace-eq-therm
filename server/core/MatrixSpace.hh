// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

#ifndef LAPLACE_EQ_THERM_SERVER_CORE_MATRIX_SPACE_HH
#define LAPLACE_EQ_THERM_SERVER_CORE_MATRIX_SPACE_HH

#include "Space.hh"
#include <random>
#include <vector>

class MatrixSpace : public Space
{
  private:
    enum class ErrorType
    {
        Success,
        BufferAllocationFailed,
        InvalidEquation,
    };

  private:
    std::vector<float> _A, _x, _b;
    std::vector<Pos>   _pos;

  public:
    MatrixSpace() = default;

  protected:
    virtual char const* GetName() noexcept override;

    virtual char const* GetErrorMessage(ErrorCode errorCode) noexcept override;

    virtual ErrorCode RunSimulation(Point const* input,
                                    float*       output,
                                    uint16_t     width,
                                    uint16_t     height) noexcept override;

  private:
    char const* GetErrorMessageInternal(ErrorType errorType) noexcept;

    ErrorType RunSimulationInternal(Point const* input,
                                    float*       output,
                                    uint16_t     width,
                                    uint16_t     height) noexcept;

    bool BuildBuffer(uint16_t width, uint16_t height) noexcept;

    bool BuildEquation(Point const* input, uint16_t width, uint16_t height) noexcept;

    void CopyNonBoundaryPoints(Point const* input,
                               float*       output,
                               uint16_t     width,
                               uint16_t     height) noexcept;

    void SolveEquation(float* output, uint16_t width, uint16_t height) noexcept;
};

#endif