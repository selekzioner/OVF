#pragma once

#include "Solver.h"
#include "Utils.h"

class NewtonSolver : public Solver {
public:
  explicit NewtonSolver(const Utils::Function& iFunction, const double iX0) : Solver(iFunction), _x0(iX0) {}

private:
  double CalcFuncVal(const double iX) const
  {
    return iX - _function(iX) / Utils::Derivative(_function, iX, _derivativeStep);
  }

  const double _x0;
  const double _derivativeStep = 0.0001;
};