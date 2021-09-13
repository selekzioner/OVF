#pragma once

#include "Utils.h"

class Solver {
public:
  explicit Solver(const Utils::Function& iFunction) : _function(iFunction) {}
  virtual double Solve() = 0;
  virtual ~Solver() = default;

protected:
  const Utils::Function& _function;
};