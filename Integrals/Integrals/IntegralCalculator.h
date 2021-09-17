#pragma once

#include <functional>
#include <vector>

using FunctionT = std::function<double(double)>;

class IntegralCalculator {
public:
  explicit IntegralCalculator(const FunctionT& iFunc, const std::vector<double>& iXVals)
    : _xValues(iXVals), _function(iFunc) {}

  virtual ~IntegralCalculator() = default;

  double Calculate()
  {
    double sum = 0;
    for (size_t ind1 = 0, ind2 = 1; ind2 < _xValues.size(); ++ind1, ++ind2) {
      sum += CalculateTerm(_xValues[ind1], _xValues[ind2]);
    }
    return sum;
  }

protected:
  virtual double CalculateTerm(double x0, double x1) = 0;

  const std::vector<double>& _xValues;
  const FunctionT _function;
};

class Trapezoidal : public IntegralCalculator {
public:

  Trapezoidal(const FunctionT& iFunc, const std::vector<double>& iXVals)
    : IntegralCalculator(iFunc, iXVals) {}

private:
  double CalculateTerm(const double x0, const double x1) override
  {
    return  std::abs(x0 - x1) * (_function(x0) + _function(x1)) / 2.0;
  }
};

class Simpson : public IntegralCalculator {
public:
  Simpson(const FunctionT& iFunc, const std::vector<double>& iXVals)
    : IntegralCalculator(iFunc, iXVals) {}

private:
  double CalculateTerm(const double x0, const double x1) override
  {
    return (x1 - x0) / 6.0 * (_function(x0) + 4.0 * _function((x0 + x1) / 2.0) + _function(x1));
  }
};
