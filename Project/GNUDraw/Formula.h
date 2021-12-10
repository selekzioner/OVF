#pragma once
#include "Primitive.h"
#include <string>
#include <sstream>

class Formula;
typedef std::shared_ptr<Formula> FormulaPtr;

class Formula : public Primitive
{
public:
  static FormulaPtr Construct(const std::string& iFormula);
  static FormulaPtr Construct(const std::string& iFormula, const std::string& iName);

  virtual void Draw(FILE* iPipe) const;

private:
  Formula(const std::string& iFormula);
  Formula(const std::string& iFormula, const std::string& iName);

  std::string _formula;
  std::string _name;
};