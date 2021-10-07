#include <cmath>
#include <iostream>
#include <sciplot/sciplot.hpp>

using namespace sciplot;

Vec Linspace(const double iX0, const double iX1, const size_t iIntervalCount)
{
  Vec oLinspace(iIntervalCount + 1);
  auto step = (iX1 - iX0) / static_cast<double>(iIntervalCount);
  for (std::size_t i = 0; i <= iIntervalCount; ++i) {
    oLinspace[i] = iX0 + static_cast<double>(i) * step;
  }
  return oLinspace;
}

Vec Linspace(const double iX0, const double iX1, const double iStep)
{
  auto numSteps = static_cast<std::size_t>((iX1 - iX0) / iStep);
  return Linspace(iX0, iX1, numSteps);
}

double Interpolate(const Vec& iYVals, const Vec& iXVals, const double iX)
{
  double result = 0;
  for (auto i = 0u; i < iXVals.size(); ++i) {
    double term = iYVals[i];
    for (auto j = 0u; j < iXVals.size(); ++j) {
      if (i != j)
        term *= (iX - iXVals[j]) / (iXVals[i] - iXVals[j]);
    }
    result += term;
  }
  return result;
}

int main()
{
  const auto nValues = Linspace(4, 15, 1.0);
  std::vector<std::pair<Vec, Vec>> results{};

  for (const auto n : nValues) {
    auto kValues = Linspace(0, n, 1.0);
    Vec xSequence(kValues.size());

    for (auto i = 0u; i < kValues.size(); ++i) {
      xSequence[i] = 1.0 + kValues[i] / n;
    }
    auto ySequence = std::log(xSequence);
    results.emplace_back(xSequence, ySequence);
  }

  Plot plot;
  for (const auto&[x, yToInterpolate] : results) {
    const auto xValues = Linspace(x[0], x[x.size() - 1], 1000u);
    const auto yLogs = std::log(xValues);

    Vec yInterpolated(yLogs.size());
    for (auto i = 0u; i < xValues.size(); ++i) {
      yInterpolated[i] = Interpolate(yToInterpolate, x, xValues[i]);
    }

    plot.drawCurveWithPoints(xValues, yInterpolated - yLogs).lineWidth(2).
    label("n = " + std::to_string(yToInterpolate.size() - 1));
  }
  plot.size(800, 500);
  plot.show();

  return 0;
}