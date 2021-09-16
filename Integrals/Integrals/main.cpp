#include <iostream>
#include <sciplot/sciplot.hpp>

#include "IntegralCalculator.h"

using namespace sciplot;

constexpr unsigned minIntervalCount = 4;
constexpr unsigned maxIntervalCount = 32;

// pi / 2
double Func1(const double iX) {
  return 1 / (1 + iX * iX);
}

// 1.29589400873
double Func2(const double iX) {
  return std::pow(iX, 1.0 / 3.0) * std::exp(std::sin(iX));
}

void Linspace(const double a, const double b, const double iStep, std::vector<double>& oVec)
{
  if (a > b)
    return;

  const auto count = static_cast<size_t>((b - a) / iStep);
  auto x = a;
  for (size_t i = 0; i <= count; ++i) {
    oVec.push_back(x);
    x += iStep;
  }
}

void CalcTrapezoidal(const FunctionT& iFunc, const double a, const double b)
{
  std::cout << "Trapezoidal:" << std::endl;
  for (unsigned i = minIntervalCount; i <= maxIntervalCount; i *= 2) {
    std::vector<double> xValues;
    Linspace(a, b, 1 / static_cast<double>(i), xValues);
    Trapezoidal calc(iFunc, xValues);
    std::cout << calc.Calculate() << std::endl;
  }
  std::cout << std::endl;
}

void CalcSimpson(const FunctionT& iFunc, const double a, const double b)
{
  std::cout << "Simpson:" << std::endl;
  for (unsigned i = minIntervalCount; i <= maxIntervalCount; i *= 2) {
    std::vector<double> xValues;
    Linspace(a, b, 1 / static_cast<double>(i), xValues);
    Simpson calc(iFunc, xValues);
    std::cout << calc.Calculate() << std::endl;
  }
  std::cout << std::endl;
}

int main()
{
  CalcTrapezoidal(Func1, -1, 1);
  CalcSimpson(Func1, -1, 1);

  return 0;
}
