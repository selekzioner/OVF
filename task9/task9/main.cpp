#define _USE_MATH_DEFINES

#include <cmath>
#include <functional>
#include <sciplot/sciplot.hpp>

/* ct1 * y(a) + ct2 * y'(a) = ct
   dt1 * y(b) + dt2 * y'(b) = dt */

constexpr double ct1 = 1;
constexpr double ct2 = 0;
constexpr double ct = 0;
constexpr double dt1 = 1;
constexpr double dt2 = 0;
constexpr double dt = 0;

std::valarray<double> GenerateLinspace(const double iT0, const double iT1, const double iStep = 1)
{
  const auto numSteps = static_cast<size_t>((iT1 - iT0) / iStep);
  std::valarray<double> vec(numSteps + 1);

  for (std::size_t i = 0; i <= numSteps; ++i) {
    vec[i] = iT0 + static_cast<double>(i) * iStep;
  }
  return vec;
}

void GenerateArray(const std::valarray<double>& iTVals, std::function<double(double)> iFunc, std::valarray<double>& oRes)
{
  oRes.resize(iTVals.size());
  for (size_t i = 0; i < iTVals.size(); i++) {
    oRes[i] = iFunc(iTVals[i]);
  }
}

double Func(const double x)
{
  return -std::sin(x);
}

void Solve(const std::valarray<double>& iXVals, std::valarray<double>& oYVals)
{
  const double h = iXVals[1] - iXVals[0];

  const double a = 1 / (h * h);
  const double b = 2 / (h * h);
  const double c = 1 / (h * h);

  std::valarray<double> eps, nu;
  eps.resize(iXVals.size());
  nu.resize(iXVals.size());
  eps[0] = -ct2 / (ct1 * h - ct2);
  nu[0] = ct * h / (ct1 * h - ct2);

  const auto secondDerivative = [](const double x) { return std::sin(x); };

  for (size_t i = 0; i < iXVals.size() - 1; ++i) {
    eps[i + 1] = c / (b - a * eps[i]);
    nu[i + 1] = (nu[i] * a - secondDerivative(iXVals[i])) / (b - a * eps[i]);
  }

  oYVals.resize(iXVals.size());
  oYVals[oYVals.size() - 1] = (dt2 * nu[nu.size() - 1] - dt * h) / (dt2 * (1 - eps[eps.size() - 1]) + dt1 * h);

  for (size_t i = iXVals.size() - 1; i > 0; --i) {
    oYVals[i - 1] = oYVals[i] * eps[i] + nu[i];
  }
}

int main()
{
  const auto xVals = GenerateLinspace(0, M_PI, 0.001);

  sciplot::Plot plot;

  std::valarray<double> yVals, yOrigin;
  Solve(xVals, yVals);
  GenerateArray(xVals, Func, yOrigin);
  //plot.drawCurve(xVals, yVals).lineWidth(2).label("y(x)");
  plot.drawCurve(xVals, yVals + yOrigin).lineWidth(2).label("difference");//"origin y(x)");

  plot.size(1000, 800);
  plot.show();

  return 0;
}
