#include <cmath>
#include <iomanip>
#include <iostream>

template <typename T>
T CalculateEpsilon()
{
  T halfEpsilon(1);
  
  while (halfEpsilon + 1 > 1) {
    halfEpsilon /= 2;
  }
  return halfEpsilon * 2;
}

template <class T>
int CalculateMantissaDigits()
{
  const auto epsilon = CalculateEpsilon<T>();
  int pow = 0;
  T value(1);

  do {
    ++pow;
    value = 1 + std::pow(T(10), -pow);
  } while (std::abs(value - 1) > std::numeric_limits<T>::min());
  return pow;
}

template <class T>
int CalculateMinPow()
{
  const auto epsilon = CalculateEpsilon<T>();
  T value(1);
  int pow = 0;

  while (std::abs(value) > std::abs(std::numeric_limits<T>::denorm_min())) {
    ++pow;
    value /= 10;
  }
  return -pow + 1;
}

template <class T>
int CalculateMaxPow()
{
  T value(1);
  int pow = 0;

  while (value * 10 > value) {
    ++pow;
    value *= 10;
  }
  return --pow;
}

template <class T>
void Compare()
{
  const auto epsilon = CalculateEpsilon<T>();
  const auto prec = std::setprecision(std::numeric_limits<T>::max_digits10);

  std::cout << "1 : " << prec << 1 << std::endl;
  std::cout << "1 + e/2: " << prec << 1 + epsilon / 2 << std::endl;
  std::cout << "1 + e: " << prec << 1 + epsilon << std::endl;
  std::cout << "1 + e + e/2: " << prec << 1 + epsilon + epsilon / 2 << std::endl;
  std::cout << std::endl;
}

int main ()
{
  const auto floatEpsilon = CalculateEpsilon<float>();
  const auto doubleEpsilon = CalculateEpsilon<double>();

  std::cout << "epsilon for float: " << floatEpsilon << std::endl;
  std::cout << "epsilon for double: " << doubleEpsilon << std::endl;
  std::cout << "numeric_limits epsilon for float: " << std::numeric_limits<float>::epsilon() << std::endl;
  std::cout << "numeric_limits epsilon for float: " << std::numeric_limits<double>::epsilon() << std::endl;
  std::cout << std::endl;

  std::cout << "number of float mantissa digits: " << CalculateMantissaDigits<float>() << std::endl;
  std::cout << "number of double mantissa digits: " << CalculateMantissaDigits<double>() << std::endl;
  std::cout << "numeric_limits number of float mantissa digits: " << std::numeric_limits<float>::max_digits10 << std::endl;
  std::cout << "numeric_limits number of double mantissa digits: " << std::numeric_limits<double>::max_digits10 << std::endl;
  std::cout << std::endl;


  std::cout << "float min pow: " << CalculateMinPow<float>() << ", float max pow: " << CalculateMaxPow<float>() << std::endl;
  std::cout << "numeric_limits float min pow: " << std::numeric_limits<float>::min_exponent10 <<
    ", numeric_limits float max pow: " << std::numeric_limits<float>::max_exponent10 << std::endl;

  std::cout << "double min pow: " << CalculateMinPow<double>() << ", double max pow: " << CalculateMaxPow<double>() << std::endl;
  std::cout << "numeric_limits double min pow: " << std::numeric_limits<double>::min_exponent10 <<
    ", numeric_limits double max pow: " << std::numeric_limits<double>::max_exponent10 << std::endl;
  std::cout << std::endl;

  //Compare<double>();
  
  return 0;
}
