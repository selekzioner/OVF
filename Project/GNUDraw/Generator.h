#pragma once
#include <vector>

class Generator
{
public:
  Generator() = delete;
  static std::vector<double> GenerateRange(double iBegin, double iEnd, double iStep);
  static std::vector<double> GenerateArray(double(*iFunc)(double), double iBegin, double iEnd, double iStep);
  static std::vector<double> GenerateArray(double(*iFunc)(double), const std::vector<double>& iRange);
};