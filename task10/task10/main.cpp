#include <sciplot/sciplot.hpp>
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

std::valarray<double> CalculateTemp(std::valarray<std::valarray<double>>& u)
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

int main()
{
  std::valarray<double> x, t;
  std::valarray<std::valarray<double>> u;

  CalculateU(u, x, t);

  sciplot::Plot plot;
  plot.drawCurve(x, CalculateTemp(u)).lineWidth(2).label("maxT(x)");

  plot.size(1000, 800);
  plot.show();

  return 0;
}
