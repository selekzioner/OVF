#define _USE_MATH_DEFINES

#include <iostream>
#include <sciplot/sciplot.hpp>
#include <math.h>

using namespace sciplot;

std::valarray<double> Linspace(const double iT0, const double iT1, const double iStep)
{
  const auto numSteps = static_cast<std::size_t>((iT1 - iT0) / iStep);
  std::valarray<double> vec(numSteps + 1);

  for (std::size_t i = 0; i <= numSteps; ++i) {
    vec[i] = iT0 + static_cast<double>(i) * iStep;
  }
  return vec;
}

double Function(const double iX)
{
  return -iX;
}

std::valarray<double> SolveEuler(const double iT0, const std::valarray<double>& iTVals)
{
  std::valarray<double> solutions(iTVals.size());
  solutions[0] = iT0;

  const double step = iTVals[1] - iTVals[0];
  for (size_t i = 0; i < iTVals.size() - 1; i++) {
    solutions[i + 1] = solutions[i] + Function(solutions[i]) * step;
  }
  return solutions;
}

std::valarray<double> SolveRungeKutta2(const double iT0, const std::valarray<double>& iTVals)
{
  std::valarray<double> solutions(iTVals.size());
  solutions[0] = iT0;

  const double step = iTVals[1] - iTVals[0];
  const double a = 0.5;

  for (size_t i = 0; i < iTVals.size() - 1; i++) {
    solutions[i + 1] = solutions[i] + step * ((1 - 1 / (2 * a)) *
      Function(solutions[i]) + 1 / (2 * a) * Function(solutions[i] + a * step * Function(solutions[i])));
  }
  return solutions;
}

std::valarray<double> SolveRungeKutta4(const double iT0, const std::valarray<double>& iTVals)
{
  std::valarray<double> solutions(iTVals.size());
  solutions[0] = iT0;

  const double step = iTVals[1] - iTVals[0];

  for (size_t i = 0; i < iTVals.size() - 1; i++) {
    const double k1 = Function(solutions[i]);
    const double k2 = Function(solutions[i] + step / 2 * k1);
    const double k3 = Function(solutions[i] + step / 2 * k2);
    const double k4 = Function(solutions[i] + step * k3);

    solutions[i + 1] = (solutions[i] + step / 6 * (k1 + 2 * k2 + 2 * k3 + k4));
  }
  return solutions;
}

int main ()
{
  const auto tVals = Linspace(0.0, 7.0, 10e-3);
  const auto eulerSolution = SolveEuler( 1.0, tVals);
  const auto rK2Solution = SolveRungeKutta2(1.0, tVals);
  const auto rK4Solution = SolveRungeKutta4(1.0, tVals);

  const auto analyticalSolution = std::exp(-tVals);

  Plot plot;

  plot.drawCurve(tVals, analyticalSolution - eulerSolution).lineWidth(2).label("Euler");
  plot.drawCurve(tVals, rK2Solution - analyticalSolution).lineWidth(2).label("RK2");
  plot.drawCurve(tVals, rK4Solution - analyticalSolution).lineWidth(2).label("RK4");

  plot.size(1000, 800);
  plot.ytics().logscale();
  plot.show();

  return 0;
}
