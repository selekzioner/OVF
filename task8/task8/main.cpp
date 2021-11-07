#include <sciplot/sciplot.hpp>

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

/* u' = F(u, v)
   v' = G(u, v) */

double F(const double iU, const double iV)
{
  return 998 * iU + 1998 * iV;
}

double G(const double iU, const double iV)
{
  return -999 * iU - 1999 * iV;
}

double u(const double t)
{
  return 4 * exp(-t) - 3 * exp(-1000 * t);
}

double v(const double t)
{
  return 3 * exp(-1000 * t) - 2 * exp(-t);
}

void EulerExplicit(const std::valarray<double>& iTVals, const double iStart1, const double iStart2, std::valarray<double>& oU, std::valarray<double>& oV)
{
  oU.resize(iTVals.size());
  oV.resize(iTVals.size());

  oU[0] = iStart1;
  oV[0] = iStart2;

  const double h = iTVals[1] - iTVals[0];

  for (size_t i = 0; i < iTVals.size() - 1; i++) {
    oU[i + 1] = oU[i] + F(oU[i], oV[i]) * h;
    oV[i + 1] = oV[i] + G(oU[i], oV[i]) * h;
  }
}

void EulerImplicit(const std::valarray<double>& iTVals, const double iStart1, const double iStart2, std::valarray<double>& oU, std::valarray<double>& oV)
{
  oU.resize(iTVals.size());
  oV.resize(iTVals.size());

  oU[0] = iStart1;
  oV[0] = iStart2;

  const double h = iTVals[1] - iTVals[0];

  const double a1 = 999 * h;
  const double a2 = 998 * h - 1;
  const double b1 = 1999 * h + 1;
  const double b2 = 1998 * h;

  for (size_t i = 0; i < iTVals.size() - 1; i++) {
    oV[i + 1] = (a2 * oV[i] + a1 * oU[i]) / (b1 * a2 - a1 * b2);
    oU[i + 1] = -(b2 * oV[i + 1] + oU[i]) / a2;
  }
}

int main()
{
  const auto tVals = GenerateLinspace(0, 2, 0.01);
  std::valarray<double> uValsE, vValsE, uValsI, vValsI, uOrigin, vOrigin;

  sciplot::Plot plot;

  /*EulerExplicit(tVals, 1, 1, uValsE, vValsE);
  plot.drawCurve(tVals, uValsE).lineWidth(3).label("expl u");
  plot.drawCurve(tVals, vValsE).lineWidth(3).label("expl v");*/

  EulerImplicit(tVals, 1, 1, uValsI, vValsI);
  plot.drawCurve(tVals, uValsI).lineWidth(3).label("impl u");
  plot.drawCurve(tVals, vValsI).lineWidth(3).label("impl v");

  GenerateArray(tVals, u, uOrigin);
  GenerateArray(tVals, v, vOrigin);
  plot.drawCurve(tVals, uOrigin).lineWidth(2).label("origin u");
  plot.drawCurve(tVals, vOrigin).lineWidth(2).label("origin v");

  plot.size(1000, 800);
  plot.show();

  return 0;
}
