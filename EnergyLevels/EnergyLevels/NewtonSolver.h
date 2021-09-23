#pragma once

#include "Solver.h"
#include "Utils.h"

class NewtonSolver : public Solver {
public:
  explicit NewtonSolver(const Utils::Function& iFunc, const double iX0) : Solver(iFunc), _x0(iX0) {}

  bool IsConverge() const
  {
    const double abs = std::abs(_function(_x0) * Utils::Derivative2(_function, _x0, _derivativeStep));
    if (abs < std::pow(Utils::Derivative(_function, _x0, _derivativeStep), 2))
      return true;
    return false;
  }

private:
  double RunImpl() override
  {
    double x = _x0;
    double x1 = _x0;

    while (abs(CalcPhi(x) - x1) > _tol) {
      x1 = CalcPhi(x);
      x = x1;
    }
    return x1;
  }

  double CalcPhi(const double iX) const
  {
    return iX - _function(iX) / Utils::Derivative(_function, iX, _derivativeStep);
  }

  const double _x0;
  const double _derivativeStep = 0.0001;
};
