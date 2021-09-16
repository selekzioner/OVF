#pragma once

#include <stdexcept>

#include "Solver.h"

class DichotomySolver : public Solver
{
public:
  explicit DichotomySolver(const Utils::Function& iFunc, const double iLBound, const double iRBound)
  : Solver(iFunc), _leftBound(iLBound), _rightBound(iRBound) {}

private:
  double RunImpl() override
  {
    auto res = _function(_rightBound) * _function(_leftBound);
    if (res > 0.0)
      throw std::out_of_range("invalid left & right bounds");

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
