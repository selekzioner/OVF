#pragma once

#include <functional>

namespace Utils
{
  using Function = std::function<double(double)>;

  inline double Derivative(const Function& iFunction, const double iX, const double iDerivativeStep)
  {
    return (iFunction(iX + iDerivativeStep) - iFunction(iX)) / iDerivativeStep;
  }
}
