#pragma once
#include <functional>
#define _USE_MATH_DEFINES
#include <math.h>

#include "../GNUDraw/GNUDraw/GNUDrawLib.h"

class ShcrodingerSolver
{
public:
  std::vector<double> x, psi_an;
  std::vector<std::vector<double>> psi;

  std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> A(std::vector<double> x, std::function<double(double)> U, int N)
  {
    double h = x[1] - x[0];

    auto a = std::vector<double>(N, -1.0 / (2 * h * h));

    a[0] = 0;

    auto b = std::vector<double>(x.size());

    for (int i = 0; i < x.size(); ++i)
    {
      b[i] = 1.0 / (h * h) + U(x[i]);
    }


    auto c = std::vector<double>(N, -1.0 / (2 * h * h));

    for (auto element : c)
    {
      element = -1.0 / (2 * h * h);
    }

    c[N - 1] = 0;

    return std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>(a, b, c);
  }

  std::vector<double> copy(std::vector<double> a)
  {
    auto b = std::vector<double>(a.size());

    for (int i = 0; i < a.size(); ++i)
    {
      b[i] = a[i];
    }

    return b;
  }

  std::vector<double> TridiagMatrixAlg(std::vector<double> a, std::vector<double> b, std::vector<double> c, std::vector<double> d, int N)
  {
    std::vector<double> y = std::vector<double>(N, 0);

    for (int i = 1; i < N; i++)
    {
      auto xi = a[i] / b[i - 1];
      a[i] = 0;
      b[i] -= xi * c[i - 1];
      d[i] -= xi * d[i - 1];
    }

    y[N - 1] = d[N - 1] / b[N - 1];
    for (int i = N - 2; i >= 0; i--)
    {
      y[i] = 1 / b[i] * (d[i] - c[i] * y[i + 1]);
    }
    return y;
  }

  double norm(std::vector<double> a)
  {
    double res = 0;
    for (int i = 0; i < a.size(); i++)
    {
      res += a[i] * a[i];
    }

    return sqrt(res);
  }

  double inner(std::vector<double> a, std::vector<double> b)
  {
    double res = 0;

    for (int i = 0; i < a.size(); i++)
    {
      res += a[i] * b[i];
    }

    return res;
  }

  void multiply(std::vector<double>& a, double b)
  {
    for (int i = 0; i < a.size(); i++)
    {
      a[i] *= b;
    }
  }

  void Orthogonalization(std::vector<double>& psi_next, std::vector<std::vector<double>>& psi_prevs)
  {
    for (auto psi : psi_prevs)
    {
      auto normal = norm(psi);
      auto innerProduct = inner(psi_next, psi);
      auto multiplier = innerProduct / normal;
      for (int i = 0; i < psi_next.size(); i++)
      {
        psi_next[i] -= psi[i] * multiplier;
      }
    }
  }

  std::vector<double> E;

  void solve(double left, double right, int N, int iterations, int levels, std::function<double(double)> U)
  {
    reallocate(N);

    E = std::vector<double>();

    psi = std::vector<std::vector<double>>();
    double dx = (right - left) / (N - 1);
    x = std::vector<double>();
    for (int i = 0; i < N; i++)
    {
      x.push_back(left + dx * i);
    }

    auto abc = A(x, U, N);
    auto a = std::get<0>(abc);
    auto b = std::get<1>(abc);
    auto c = std::get<2>(abc);

    std::vector<double> psi_0 = std::vector<double>();
    for (int i = 0; i < N; i++)
    {
      psi_0.push_back(1.0 + ((double)i / (N - 1)));
    }

    for (int i = 0; i < levels; i++)
    {
      auto psiNext = std::vector<double>(psi_0);
      Orthogonalization(psiNext, psi);
      std::vector<double> psiPrev;

      for (int k = 0; k < iterations; k++)
      {
        psiPrev = psiNext;
        psiNext = TridiagMatrixAlg(copy(a), copy(b), copy(c), copy(psiNext), N);
        Orthogonalization(psiNext, psi);
      }

      auto E0 = norm(psiPrev) / norm(psiNext);
      E.push_back(E0);
      multiply(psiNext, 1.0 / norm(psiNext));
      psi.push_back(psiNext);
    }

    for (int i = 0; i < N; i++)
    {
      psi_an[i] = exp(-x[i] * x[i] / 2);
    }

    multiply(psi_an, 1.0 / norm(psi_an));
  }

  void reallocate(int N)
  {
    psi_an = std::vector<double>(N, 0);
  }
};

int main()
{
  std::function<double(double)> U = [](double x) {return x * x / 2; };

  int N = 2000;
  int iter = 21;
  int levels = 4;

  auto solver = ShcrodingerSolver();
  solver.solve(-10, 10, N, iter, levels, U);

  GNUDrawer gnuDrawer;
  auto arr = Arrays::Construct();
  arr->Add("x", solver.x);

  for (int i = 0; i < solver.psi.size(); ++i)
  {
    arr->Add("psi " + std::to_string(i), solver.psi[i]);
  }

  arr->Add("psi_an", solver.psi_an);
  gnuDrawer.Add(arr);
  gnuDrawer.Draw();

  return 0;
}