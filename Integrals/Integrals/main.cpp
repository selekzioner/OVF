#define _USE_MATH_DEFINES

#include <iostream>
#include <sciplot/sciplot.hpp>
#include <math.h>

#include "IntegralCalculator.h"

using namespace sciplot;

constexpr unsigned minIntervalCount = 2;
constexpr unsigned maxIntervalCount = 16384;

// pi / 2
double Func1(const double iX)
{
  return 1.0 / (1.0 + iX * iX);
}

// 1.29589400873
double Func2(const double iX)
{
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

void CalcTrapezoidal(const FunctionT& iFunc, const double a, const double b, std::vector<double>& oResults)
{
  std::cout << "Trapezoidal:" << std::endl;
  for (unsigned i = minIntervalCount; i <= maxIntervalCount; i *= 2) {
    std::cout << i << " intervals: ";
    std::vector<double> xValues;
    Linspace(a, b, std::abs(b - a) / static_cast<double>(i), xValues);
    Trapezoidal calc(iFunc, xValues);
    const auto res = calc.Calculate();
    oResults.push_back(res);
    std::cout << res << std::endl;
  }
  std::cout << std::endl;
}

void CalcSimpson(const FunctionT& iFunc, const double a, const double b, std::vector<double>& oResults)
{
  std::cout << "Simpson:" << std::endl;
  for (unsigned i = minIntervalCount; i <= maxIntervalCount; i *= 2) {
    std::cout << i << " intervals: ";
    std::vector<double> xValues;
    Linspace(a, b, std::abs(b - a) / static_cast<double>(i), xValues);
    Simpson calc(iFunc, xValues);
    const auto res = calc.Calculate();
    oResults.push_back(res);
    std::cout << res << std::endl;
  }
  std::cout << std::endl;
}

int main()
{
  const std::vector<double> degrees{ 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };
  Plot plot;
  plot.size(1000, 700);
  std::vector<double> vals1, vals2;

  /*std::cout << "Function1:" << std::endl;
  CalcTrapezoidal(Func1, -1, 1, vals1);
  std::for_each(vals1.begin(), vals1.end(), [](double& res) {
    res = std::abs(M_PI_2 - res);
    });
  plot.drawCurveWithPoints(degrees, vals1).lineWidth(10).label("Trapezioidal");

  CalcSimpson(Func1, -1, 1, vals2);
  std::for_each(vals2.begin(), vals2.end(), [](double& res) {
    res = std::abs(M_PI_2 - res);
    });
  plot.drawCurveWithPoints(degrees, vals2).lineWidth(5).label("Simpson");*/

  std::cout << "Function2:" << std::endl;
  CalcTrapezoidal(Func2, 0, 1, vals1);
  CalcSimpson(Func2, 0, 1, vals2);
  const double v1 = vals1[vals1.size() - 1];
  const double v2 = vals2[vals2.size() - 1];
  std::for_each(vals1.begin(), vals1.end(), [=](double& res) {
    res = std::abs(v1 - res);
    });
  plot.drawCurveWithPoints(degrees, vals1).lineWidth(10).label("Trapezioidal");

  std::for_each(vals2.begin(), vals2.end(), [=](double& res) {
    res = std::abs(v2 - res);
    });
  plot.drawCurveWithPoints(degrees, vals2).lineWidth(5).label("Simpson");

  plot.ytics().logscale();
  plot.xtics().logscale();
  plot.show();

  return 0;
}
