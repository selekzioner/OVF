#define _USE_MATH_DEFINES

#include <iostream>
#include <sciplot/sciplot.hpp>
#include <math.h>

using namespace sciplot;

constexpr double a = 10;
constexpr double b = 2;
constexpr double c = 2;
constexpr double d = 10;

std::valarray<double> Linspace(const double iT0, const double iT1, const double iStep = 1)
{
  const auto numSteps = static_cast<size_t>((iT1 - iT0) / iStep);
  std::valarray<double> vec(numSteps + 1);

  for (std::size_t i = 0; i <= numSteps; ++i) {
    vec[i] = iT0 + static_cast<double>(i) * iStep;
  }
  return vec;
}

double XFunction(const double x, const double y)
{
  return a * x - b * x * y;
}

double YFunction(const double x, const double y)
{
  return c * x * y - d * y;
}

void SolveRK2(const double iX0, const double iY0, const double iStep, std::valarray<double>& oXVals, std::valarray<double>& oYVals)
{
  const std::valarray<double> tVals = Linspace(0, 1, iStep);

  oXVals.resize(tVals.size());
  oYVals.resize(tVals.size());
  oXVals[0] = iX0; oYVals[0] = iY0;

  for (unsigned int i = 0; i < tVals.size() - 1; ++i) {
    oXVals[i + 1] = oXVals[i] + iStep * XFunction(oXVals[i] +
      iStep / 2.0 * XFunction(oXVals[i], oYVals[i]), oYVals[i] + iStep / 2.0);

    oYVals[i + 1] = oYVals[i] + iStep * YFunction(
      oXVals[i] + iStep / 2.0, oYVals[i] + iStep / 2.0 * YFunction(oXVals[i], oYVals[i]));
  }
}

int main()
{
  const auto x0Vals = Linspace(1, 3);
  const auto y0Vals = Linspace(1, 3);

  Plot plot;
  for (const auto& x0 : x0Vals) {
    for (const auto& y0 : y0Vals) {
      std::valarray<double> xVals, yVals;
      SolveRK2(x0, y0, 0.0001, xVals, yVals);

      plot.drawCurve(xVals, yVals).lineWidth(2).label(std::to_string(x0) + ", " + std::to_string(y0));
    }
  }

  plot.size(1000, 800);
  plot.show();

  return 0;
}
