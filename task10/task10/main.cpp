#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>

#include "sciplot/sciplot.hpp"

std::valarray<std::valarray<std::valarray<double>>> u;

std::valarray<double> x;
std::valarray<double> y;
std::valarray<double> t;

double L = 1;

double f(double x, double y, double t) {
  return (1 - x * x / L / L)*(1 - y * y / L / L) + (y / L + 1)*(1 - y * y / L / L)*sin(2 * M_PI*t);
}

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

/*double xBoundFunction(double t) {
  return 0;
}*/

double yBoundFunction(double t) {
  return 0;
}

double tStartDistribution(double x, double y) {
  return 0;
}

double aX, aY;
double bX, bY;
double cX, cY;

double X, Y;

double diX(size_t ix, size_t iy, size_t it)
{
  return Y * u[ix][iy + 1][it] + (1 - 2 * Y)*u[ix][iy][it] + Y * u[ix][iy - 1][it] + f(x[ix], y[iy], t[it]);
}

double diY(size_t ix, size_t iy, size_t it)
{
  return X * u[ix + 1][iy][it] + (1 - 2 * X)*u[ix][iy][it] + X * u[ix - 1][iy][it] + f(x[ix], y[iy], t[it]);
}

void init()
{
  x = GenerateLinspace(0, L, 0.03);
  y = GenerateLinspace(0, L, 0.03);
  t = GenerateLinspace(0, 1, 0.0005);

  u.resize(x.size());
  for (size_t ix = 0; ix < x.size(); ix++) {
    u[ix].resize(y.size());
    for (size_t iy = 0; iy < y.size(); iy++) {
      u[ix][iy].resize(t.size());
      u[ix][iy][0] = tStartDistribution(x[ix], y[iy]);
    }
  }

  /*for (size_t iy = 0; iy < y.size(); iy++) {
    for (size_t it = 0; it < t.size(); it++) {
      u[0][iy][it] = xBoundFunction(t[it]);
      u[x.size() - 1][iy][it] = xBoundFunction(t[it]);
    }
  }*/

  for (size_t ix = 0; ix < x.size(); ix++) {
    for (size_t it = 0; it < t.size(); it++) {
      u[ix][0][it] = yBoundFunction(t[it]);
      u[ix][y.size() - 1][it] = yBoundFunction(t[it]);
    }
  }

  double dT = t[1] - t[0];
  double dX = x[1] - x[0];
  double dY = y[1] - y[0];

  X = dT / (2 * dX*dX);
  Y = dT / (4 * dY*dY);

  aX = -X;
  bX = 1 + 2 * X;
  cX = -X;

  aY = -Y;
  bY = 1 + 2 * Y;
  cY = -Y;
}

double csharpX(size_t i)
{
  if (i == 1) return cX / bX;
  return cX / (bX - aX * csharpX(i - 1));
}

double csharpY(size_t i)
{
  if (i == 1) return cY / bY;
  return cY / (bY - aY * csharpY(i - 1));
}

double dsharpX(size_t ix, size_t iy, size_t it)
{
  if (ix == 1) return diX(ix, iy, it - 1) / bX;
  return (diX(ix, iy, it - 1) - aX * dsharpX(ix - 1, iy, it)) / (bX - aX * csharpX(ix - 1));
}

double dsharpY(size_t ix, size_t iy, size_t it)
{
  if (iy == 1) return diY(ix, iy, it - 1) / bY;
  return (diY(ix, iy, it - 1) - aY * dsharpY(ix, iy - 1, it)) / (bY - aY * csharpY(iy - 1));
}

void U_n_plus_1_2(size_t iy, size_t it) {
  std::valarray<double> csharpVec;
  std::valarray<double> dsharpVec;
  std::valarray<double> xn;

  size_t N = x.size() - 2;

  csharpVec.resize(N - 1);
  for (size_t i = 1; i < N; i++) {
    csharpVec[i - 1] = csharpX(i);
  }

  dsharpVec.resize(N);
  xn.resize(N);
  for (size_t i = 1; i <= N; i++) {
    dsharpVec[i - 1] = dsharpX(i, iy, it);
    xn[i - 1] = dsharpVec[i - 1];
  }

  for (size_t i = N - 1; i >= 1; i--) {
    xn[i - 1] = dsharpVec[i - 1] - csharpVec[i - 1] * xn[i];
  }

  for (size_t i = 1; i < x.size() - 1; i++) {
    u[i][iy][it] = xn[i - 1];
  }
}

void U_n_plus_1(size_t ix, size_t it) {
  std::valarray<double> csharpVec;
  std::valarray<double> dsharpVec;
  std::valarray<double> yn;

  size_t N = y.size() - 2;

  csharpVec.resize(N - 1);
  for (size_t i = 1; i < N; i++) {
    csharpVec[i - 1] = csharpY(i);
  }

  dsharpVec.resize(N);
  yn.resize(N);
  for (size_t i = 1; i <= N; i++) {
    dsharpVec[i - 1] = dsharpY(i, ix, it);
    yn[i - 1] = dsharpVec[i - 1];
  }

  for (size_t i = N - 1; i >= 1; i--) {
    yn[i - 1] = dsharpVec[i - 1] - csharpVec[i - 1] * yn[i];
  }

  for (size_t i = 1; i < x.size() - 1; i++) {
    u[ix][i][it] = yn[i - 1];
  }
}

void calculate()
{
  size_t ts = t.size() - 1;

  for (size_t it = 1; it < t.size(); it++) {

    std::cout << "Calculating " << it << "/" << ts << "\r";

    for (size_t iy = 1; iy < y.size() - 1; iy++) {
      U_n_plus_1_2(iy, it);
    }

    it++;

    for (size_t ix = 1; ix < x.size() - 1; ix++) {
      U_n_plus_1(ix, it);
    }
  }
}

std::valarray<std::valarray<double>> temperatureMap(size_t it) {
  std::valarray<std::valarray<double>> tempMap;

  tempMap.resize(u.size());
  for (auto it = std::begin(tempMap); it != std::end(tempMap); ++it) {
    it->resize(u[0].size());
  }

  for (size_t ix = 0; ix < tempMap.size(); ix++) {
    for (size_t iy = 0; iy < tempMap[ix].size(); iy++) {
      tempMap[ix][iy] = u[ix][iy][it];
    }
  }

  return tempMap;
}

int main()
{
  init();
  calculate();
  FILE *gPipe = _popen("gnuplot.exe -persist", "w");

  GNUDrawer gnuDrawer;
  for (int i = t.size() - 1; i >= 0; i--) {
    std::vector<std::vector<double>> temp = temperatureMap(i);
    gnuDrawer.Add(Matrix::Construct(temp));
  }
  gnuDrawer.Draw();

  return 0;
}

/*#include <sciplot/sciplot.hpp>
#include <vector>

constexpr double L = 1;

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

double u_x_0(double x)
{
  return x * (1 - x / L)*(1 - x / L);
}

double CalculateDi(std::valarray<std::valarray<double>>& u, const size_t n, const size_t i, const double r)
{
  return r * u[i + 1][n] + (1 - 2 * r) * u[i][n] + r * u[i - 1][n];
}

double CalcucateC(size_t i, const double a, const double b, const double c)
{
  if (i == 1)
    return c / b;
  return c / (b - a * CalcucateC(i - 1, a, b, c));
}

double CalculateD(std::valarray<std::valarray<double>>& u, size_t n, size_t i,
  const double a, const double b, const double c, const double r)
{
  if (i == 1)
    return CalculateDi(u, n - 1, i, r) / b;
  return (CalculateDi(u, n - 1, i, r) - a * CalculateD(u, n, i - 1, a, b, c, r)) / (b - a * CalcucateC(i - 1, a, b, c));
}

void CalculateU(std::valarray<std::valarray<double>>& u, std::valarray<double>& x, std::valarray<double>& t)
{
  x = GenerateLinspace(0, L, 0.02);
  t = GenerateLinspace(0, 1, 0.02);

  u.resize(x.size());
  for (size_t i = 0; i < u.size(); i++) {
    u[i].resize(t.size());
    u[i][0] = u_x_0(x[i]);
  }

  const double r = (t[1] - t[0]) / (2 * (x[1] - x[0]) * (x[1] - x[0]));
  const double a = -r;
  const double b = (1 + 2 * r);
  const double c = -r;

  for (size_t n = 1; n < t.size(); n++) {
    std::vector<double> csharpVec;
    std::vector<double> dsharpVec;
    std::vector<double> tn;

    size_t N = x.size() - 2;

    for (size_t i = 1; i < N; i++) {
      csharpVec.push_back(CalcucateC(i, a, b, c));
    }
    for (size_t i = 1; i <= N; i++) {
      dsharpVec.push_back(CalculateD(u, n, i, a, b, c, r));
      tn.push_back(dsharpVec[i - 1]);
    }
    for (size_t i = N - 1; i >= 1; i--) {
      tn[i - 1] = dsharpVec[i - 1] - csharpVec[i - 1] * tn[i];
    }
    for (size_t i = 1; i < u.size() - 1; i++) {
      u[i][n] = tn[i - 1];
    }
  }
}

void U_n_plus_1(size_t n, std::valarray<double>& x, std::valarray<double>& t, std::valarray< std::valarray<double>>& u) {
  std::valarray<double> cVals;
  std::valarray<double> dVals;
  std::valarray<double> tn;

  const double r = (t[1] - t[0]) / (2 * (x[1] - x[0]) * (x[1] - x[0]));
  const double a = -r;
  const double b = (1 + 2 * r);
  const double c = -r;

  size_t N = x.size() - 2;
  cVals.resize(N - 1);
  dVals.resize(N);
  tn.resize(N);

  for (size_t i = 1; i < N; i++) {
    dVals[i - 1] = CalcucateC(i, a, b, c);
  }
  for (size_t i = 1; i <= N; i++) {
    dVals[i - 1] = CalculateD(u, n, i, a, b, c, r);
    tn[i - 1] = dVals[i - 1];
  }
  for (size_t i = N - 1; i >= 1; i--) {
    tn[i - 1] = dVals[i - 1] - cVals[i - 1] * tn[i];
  }
  for (size_t i = 1; i < u.size() - 1; i++) {
    u[i][n] = tn[i - 1];
  }
}

std::valarray<double> CalculateMaxTemp(std::valarray<std::valarray<double>>& u)
{
  std::valarray<double> maxT;
  maxT.resize(u[0].size());
  for (size_t x = 0; x < u.size(); x++) {
    for (size_t t = 0; t < u[0].size(); t++) {
      maxT[t] = std::fmax(u[x][t], maxT[t]);
    }
  }
  return maxT;
}

std::valarray<double> CalculateMaxTemp1(std::valarray<std::valarray<double>>& u, size_t iT)
{
  std::valarray<double> maxT;
  maxT.resize(u.size());
  for (size_t x = 0; x < u.size(); x++) {
    maxT[x] = u[x][iT];
  }
  return maxT;
}

int main()
{
  std::valarray<double> x, t;
  std::valarray<std::valarray<double>> u;

  CalculateU(u, x, t);

  sciplot::Plot plot;
  plot.size(1000, 800);
  for (size_t i = 0; i < t.size(); i++) {
    plot.drawCurve(x, CalculateMaxTemp1(u, i)).lineWidth(2);
  }

  plot.size(1000, 800);
  plot.drawCurve(x, CalculateMaxTemp(u)).lineWidth(2).label("maxT(x)");
  plot.show();

  return 0;
}*/
