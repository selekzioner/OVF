#include "Generator.h"

std::vector<double> Generator::GenerateRange(double iBegin, double iEnd, double iStep)
{
  std::vector<double> oVector = {};
  for(double _current = iBegin; _current <= iEnd; _current += iStep)
    oVector.push_back(_current);

  return oVector;
}

std::vector<double> Generator::GenerateArray(double(*iFunc)(double), double iBegin, double iEnd, double iStep)
{
  std::vector<double> oVector = {};
  for (double _current = iBegin; _current <= iEnd; _current += iStep)
    oVector.push_back(iFunc(_current));

  return oVector;
}

std::vector<double> Generator::GenerateArray(double(*iFunc)(double), const std::vector<double>& iRange)
{
  std::vector<double> oVector = {};
  for (auto it = iRange.begin(); it != iRange.end(); ++it)
    oVector.push_back(iFunc(*it));

  return oVector;
}
