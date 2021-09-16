#include <sciplot/sciplot.hpp>
#include <iostream>

#include "DichotomySolver.h"
#include "SimpleIterationsSolver.h"
#include "NewtonSolver.h"

using namespace sciplot;

double WaveFunction(const double x)
{
  const double u0 = 1000;
  const double a = 10;

  return -std::sqrt(-u0 / x - 1.0)
    + 1.0 / std::tan(std::sqrt(2.0 * a * a * u0 * (1.0 + x / u0)));
}

void PlotFunc(const Utils::Function& iFunc, const double a, const double b)
{
  const Vec x = Utils::Linspace(a, b, 1000);
  const Vec y = Utils::CalculateFunction(x, iFunc);

  Plot plot;
  plot.size(1000, 700);

  // axis x
  Vec y0(x.size());
  std::fill(std::begin(y0), std::end(y0), 0.0);

  // plot function
  plot.drawCurve(x, y0).lineColor("black").lineWidth(1).label("");
  plot.drawCurve(x, y).lineWidth(3).label("WaveFunction");

  DichotomySolver dSolver(iFunc, -4.9, -3.0);
  const auto dRoot = dSolver.Solve();
  plot.drawCurveWithPoints(Vec{ dRoot }, Vec{ 0.0 }).lineWidth(10).label("Dichotomy").lineColor("red");
  std::cout << "Dichotomy: " << dRoot << std::endl;

  SimpleIterationsSolver siSolver(iFunc, -4.9);
  const auto siRoot = siSolver.Solve();
  plot.drawCurveWithPoints(Vec{ siRoot }, Vec{ 0.0 }).lineWidth(7).label("SimpleIterations").lineColor("blue");
  std::cout << "SimpleIterations: " << siRoot << std::endl;

  NewtonSolver nSolver(iFunc, -4.9);
  const auto nRoot = nSolver.Solve();
  plot.drawCurveWithPoints(Vec{ nRoot }, Vec{ 0.0 }).lineWidth(3).label("Newton").lineColor("green");
  std::cout << "Newton: " << nRoot << std::endl;

  plot.show();
}

int main ()
{
  PlotFunc(WaveFunction, -8, 0);
  return 0;
}
