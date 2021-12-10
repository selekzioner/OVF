#pragma once
#include <vector>
#include <functional>

namespace Utils
{
  static std::vector<double> GenerateRange(double iBegin, double iEnd, double iStep)
  {
    std::vector<double> oVector = {};
    for (double _current = iBegin; _current < iEnd + iStep; _current += iStep)
      oVector.push_back(_current);

    return oVector;
  }

  static std::vector<double> GenerateGeometricProgression(double iBegin, double iEnd, double iK)
  {
    std::vector<double> oVector = {};
    for (double _current = iBegin; _current <= iEnd; _current *= iK)
      oVector.push_back(_current);

    return oVector;
  }

  static std::vector<double> GenerateArray(std::function<double(double)> iFunc, double iBegin, double iEnd, double iStep)
  {
    std::vector<double> oVector = {};
    for (double _current = iBegin; _current <= iEnd; _current += iStep)
      oVector.push_back(iFunc(_current));

    return oVector;
  }

  static std::vector<double> GenerateArray(std::function<double(double)> iFunc, const std::vector<double>& iRange)
  {
    std::vector<double> oVector = {};
    for (auto it = iRange.begin(); it != iRange.end(); ++it)
      oVector.push_back(iFunc(*it));

    return oVector;
  }

  static double Derivative(std::function<double(double)> iFunc, double iX, double iDelta = 0.01)
  {
    return (iFunc(iX + iDelta) - iFunc(iX)) / iDelta;
  }

  static std::vector<double> DiffVector(const std::vector<double>& iVec1, const std::vector<double>& iVec2)
  {
    std::vector<double> resVec;
    for (size_t i = 0; i < iVec1.size(); i++)
    {
      resVec.push_back(std::abs(iVec1[i] - iVec2[i]));
    }
    return resVec;
  }
};