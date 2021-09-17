#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include "IntegralCalculator.h"

void Linspace(const double a, const double b, const unsigned iIntervalCount, std::vector<double>& oVec)
{
  oVec.resize(iIntervalCount + 1);
  const auto step = (b - a) / static_cast<double>(iIntervalCount);
  for (std::size_t i = 0; i <= iIntervalCount; ++i) {
    oVec[i] = a + static_cast<double>(i) * step;
  }
}

double BesselFunction(const double x, const int m)
{
  const std::function<double(double)> func = [&](const double t) {
    return std::cos(m * t - x * std::sin(t));
  };

  std::vector<double> xValues;
  Linspace(0, M_PI, 500, xValues);
  Simpson intCalc(func, xValues);
  return intCalc.Calculate() / M_PI;
}

double BesselFunctionDerivative(const double x, const int m)
{
  const auto derivativeStep = 10e-9;
  return (BesselFunction(x + derivativeStep, m) - BesselFunction(x, m)) / derivativeStep;
}

int main()
{
  const double x = 0.2;
  const double result = BesselFunctionDerivative(x, 0) + BesselFunction(x, 1);

  std::cout << result << std::endl;

  return 0;
}