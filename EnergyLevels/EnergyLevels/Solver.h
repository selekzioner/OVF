#pragma once

#include "Utils.h"

class Solver {
public:
  explicit Solver(const Utils::Function& iFunction) : _function(iFunction) {}
  virtual ~Solver() = default;

  double Solve()
  {
    return RunImpl();
  }

protected:
  virtual double RunImpl() = 0;

  const Utils::Function& _function;
  const double _tol = 100000 * std::numeric_limits<double>::epsilon();
};