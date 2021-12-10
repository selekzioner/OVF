#pragma once
#include "Primitive.h"
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>

class Matrix;
typedef std::shared_ptr<Matrix> MatrixPtr;

class Matrix : public Primitive
{
public:
  static MatrixPtr Construct(const std::vector<std::vector<double>>& iValues);

  void WriteToFile(const std::string& iFile) const;
  virtual void Draw(FILE* iPipe) const;

  static size_t _arraySize;
private:
  Matrix(const std::vector<std::vector<double>>& iValues);
  void DrawMatrix(FILE* iPipe) const;

  std::string _tmpFileName;
  std::vector<std::vector<double>> _matrix = {};
};
