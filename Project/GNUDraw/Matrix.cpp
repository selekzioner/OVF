#include "Matrix.h"

size_t Matrix::_arraySize = 0;
Matrix::Matrix(const std::vector<std::vector<double>>& iValues)
{
  Matrix::_arraySize++;
  _tmpFileName = "tmpMatrix" + std::to_string(Matrix::_arraySize) + ".json";

  size_t size = iValues[0].size();
  for (auto it = iValues.begin(); it != iValues.end(); ++it) {
    if (it->size() != size)
      throw "Collumn sizes are not equal";
  }
  _matrix = iValues;
}

MatrixPtr Matrix::Construct(const std::vector<std::vector<double>>& iValues)
{
  return MatrixPtr(new Matrix(iValues));
}

void Matrix::WriteToFile(const std::string & iFile) const
{
  std::ofstream fstream;
  fstream.open(iFile.data(), 'w');
  if (!fstream)
    throw "Can not open file";

  for (auto it1 = _matrix.begin(); it1 != _matrix.end(); ++it1) {
    for (auto it2 = it1->begin(); it2 != it1->end(); ++it2) {
      fstream << std::left << std::setw(20) << *it2 << " ";
    }
    fstream << std::endl;
  }

  fstream.close();
}

void Matrix::Draw(FILE * iPipe) const
{
  WriteToFile(_tmpFileName);
  DrawMatrix(iPipe);
}

void Matrix::DrawMatrix(FILE * iPipe) const
{
  std::stringstream command;
  command << "replot '" << _tmpFileName << "' matrix with image\n";
  fprintf(iPipe, command.str().data());
}