#include "Formula.h"


Formula::Formula(const std::string & iFormula)
  : _formula(iFormula)
{
}

Formula::Formula(const std::string & iFormula, const std::string & iName)
  : _formula(iFormula), _name(iName)
{
}

FormulaPtr Formula::Construct(const std::string & iFormula)
{
  return FormulaPtr(new Formula(iFormula));
}

FormulaPtr Formula::Construct(const std::string & iFormula, const std::string & iName)
{
  return FormulaPtr(new Formula(iFormula, iName));
}

void Formula::Draw(FILE * iPipe) const
{
  std::stringstream command;
  if (_name.empty())
    command << "replot " << _formula << "\n";
  else
    command << "replot " << _formula << " w l t '" << _name << "(x)'\n";
  fprintf(iPipe, command.str().data());
}
