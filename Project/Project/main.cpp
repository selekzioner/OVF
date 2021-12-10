#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "sciplot/sciplot.hpp"
#include "GNUDraw/GNUDrawLib.h"

std::valarray<std::valarray<std::valarray<double>>> u;

std::valarray<double> x;
std::valarray<double> y;
std::valarray<double> t;

double L = 1;

double f(double x, double y, double t) {
  return (1 - x * x / L / L)*(1 - y * y / L / L) + (y / L + 1)*(1 - y * y / L / L)*sin(2 * M_PI * t);
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

void Init()
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

void Calculate()
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
  Init();
  Calculate();

  GNUDrawer gnuDrawer;
  for (int i = t.size() - 1; i >= 0; i = i - 100) {
    std::vector<std::vector<double>> temp;
    const auto t = temperatureMap(i);
    temp.resize(t.size());

    for (size_t k = 0; k < t.size(); k++) {
      temp[k].resize(t[0].size());
      for (size_t j = 0; j < t[0].size(); j++) {
        temp[k][j] = t[k][j];
      }
    }

    gnuDrawer.Add(Matrix::Construct(temp));
  }
  gnuDrawer.Draw();

  return 0;
}
