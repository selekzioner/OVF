#pragma once

#include <stdexcept>

#include "Solver.h"

class DichotomySolver : public Solver
{
public:
  explicit DichotomySolver(const Utils::Function& iFunc, const double iLBound, const double iRBound)
  : Solver(iFunc), _leftBound(iLBound), _rightBound(iRBound) {}

  bool IsConverge() const
  {
    if (_function(_rightBound) * _function(_leftBound) >= 0.0)
      return false;
    return true;
  }

private:
  double RunImpl() override
  {
    double x = 0;
    while (_rightBound - _leftBound > _tol) {
      x = (_leftBound + _rightBound) / 2.0;
      if (_function(_leftBound) * _function(x) < 0.0)
        _rightBound = x;
      else
        _leftBound = x;
    }
    return x;
  }

  double _leftBound, _rightBound;
};
