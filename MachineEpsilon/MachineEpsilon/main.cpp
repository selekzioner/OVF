#include <iomanip>
#include <iostream>

template <typename T>
T CalculateEpsilon(T iMin = 0.0, T iMax = 1.0)
{
  T leftBound = iMax, rightBound = iMin, value;
  auto one = T(1), two = T(2);

  // Binary search
  while (true) {
    value = (leftBound + rightBound) / two;
    if (1 + value / two != one)
      leftBound = value;
    else if (one + value == one)
      rightBound = value;
    else
      return value;
  }
}

template <class T>
int CalculateMantissaDigits()
{
  auto one = T(1), ten = T(10);
  int pow = 0;

  while (true) {
    ++pow;
    T value = one + std::pow(ten, -pow);
    if (value == one)
      return pow;
  }
}

template <class T>
int CalculateMinPow()
{
  auto one = T(1), zero = T(0), ten = T(10);
  int pow = 0;
  T value(1);

  while (value != zero) {
    ++pow;
    value /= ten;
  }
  return -pow;
}

template <class T>
int CalculateMaxPow()
{
  auto one = T(1), ten = T(10);
  int pow = 0;
  T value(1);

  while (value < ten * value) {
    ++pow;
    value /= ten;
  }
  return pow;
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
  std::cout << std::endl;

  std::cout << "number of float mantissa digits: " << CalculateMantissaDigits<float>() << std::endl;
  std::cout << "number of double mantissa digits: " << CalculateMantissaDigits<double>() << std::endl;
  std::cout << std::endl;

  std::cout << "float min pow: " << CalculateMinPow<float>() << ", float max pow : " << CalculateMaxPow<float>() << std::endl;
  std::cout << "double min pow: " << CalculateMinPow<double>() << ", double max pow : " << CalculateMaxPow<double>() << std::endl;
  std::cout << std::endl;

  Compare<float>();
  Compare<double>();
  
  return 0;
}
