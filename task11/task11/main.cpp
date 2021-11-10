#define N 1000

#include <iostream>
#include <fstream>
#include <cmath>
#include <sciplot/sciplot.hpp>

using namespace std;

double l = 0, r = 7, h = (r - l) / N;
double energy = 0, product, normalization, previous, epsilon = 0.0001;

int main()
{
  double x[N + 1], y[N + 1], a[N + 1], b[N + 1], c[N + 1], u[N + 1];

  for (int i = 0; i <= N; i++) {
    x[i] = 1;
    u[i] = (l + i * h) * (l + i * h) / 2;
    a[i] = -1 / (2 * h  * h);
    b[i] = -1 / (2 * h * h);
    c[i] = 1 / (h * h) + u[i] - energy;
  }

  x[0] = 0; x[N] = 0;
  a[0] = 0; a[N] = 0;
  b[0] = 0; b[N] = 0;
  c[0] = 1; c[N] = 1;

  double alpha[N + 1], beta[N + 1];
  alpha[1] = 1; beta[1] = 0;

  previous = energy + 2 * epsilon;
  while (abs(previous - energy) > epsilon) {
    for (int i = 1; i < N; i++) {
      alpha[i + 1] = -b[i] / (a[i] * alpha[i] + c[i]);
      beta[i + 1] = (x[i] - a[i] * beta[i]) / (a[i] * alpha[i] + c[i]);
    }

    y[N] = (x[N] - a[N] * beta[N]) / (c[N] + a[N] * alpha[N]);
    normalization = y[N] * y[N];

    for (int i = N - 1; i >= 0; i--) {
      y[i] = alpha[i + 1] * y[i + 1] + beta[i + 1];
      normalization += y[i] * y[i];
    }

    product = 0;
    for (int i = 0; i <= N; i++) {
      product += x[i] * y[i];
    }
    previous = energy;
    energy += 1. / product;

    for (int i = 0; i <= N; i++) {
      x[i] = y[i] / sqrt(normalization);
    }

    cout << energy << endl;

    for (int i = 0; i <= N; i++) {
      c[i] = 1. / (h * h) + u[i] - energy;
    }
  }

  ofstream data;
  data.open("data.txt");

  for (int i = 0; i <= N; i++) {
    data << l + i * h << "	" << abs(x[i]) << endl;
  }

  data.close();

  return 0;
}
