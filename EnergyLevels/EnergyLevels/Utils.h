#pragma once

#include <functional>

namespace Utils
{
  using Function = std::function<double(double)>;

  inline double Derivative(const Function& iFunction, const double iX, const double iDerivativeStep)
  {
    return (iFunction(iX + iDerivativeStep) - iFunction(iX)) / iDerivativeStep;
  }

  inline sciplot::Vec Linspace(const double iX0, const double iX1, const size_t iCount)
  {
    sciplot::Vec oLinspace(iCount + 1);
    const auto step = (iX1 - iX0) / static_cast<double>(iCount);
    for (std::size_t i = 0; i <= iCount; ++i) {
      oLinspace[i] = iX0 + static_cast<double>(i) * step;
    }
    return oLinspace;
  }

  inline sciplot::Vec CalculateFunction(const sciplot::Vec& iX, const Utils::Function&  iFunc) {
    sciplot::Vec oFunctionValues(iX.size());
    std::transform(std::begin(iX),
      std::end(iX), std::begin(oFunctionValues), iFunc);
    return oFunctionValues;
  }
}
