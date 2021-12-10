#include "Arrays.h"


Collumn::Collumn(const std::string & iName, const std::vector<double>& iValues)
  : _name(iName), _values(iValues)
{
}

size_t Collumn::Size() const
{
  return _values.size();
}

size_t Arrays::_arraySize = 0;
Arrays::Arrays()
{
  Arrays::_arraySize++;
  _tmpFileName = "tmp" + std::to_string(Arrays::_arraySize) + ".json";
}

ArraysPtr Arrays::Construct()
{
  return ArraysPtr(new Arrays());
}

void Arrays::Add(const std::string & iName, const std::vector<double>& iValues)
{
  if (_collumn.empty())
    _collumnSize = iValues.size();
  else if (iValues.size() != _collumnSize)
    throw "Collumn sizes are not equal";

  _collumn.push_back(Collumn(iName, iValues));
}

void Arrays::WriteToFile(const std::string & iFile) const
{
  std::ofstream fstream;
  fstream.open(iFile.data(), 'w');
  if (!fstream)
    throw "Can not open file";

  for (auto it = _collumn.begin(); it != _collumn.end(); ++it)
    fstream << std::left << std::setw(20) << it->_name << " ";
  fstream << std::endl;

  for (size_t i = 0; i < _collumnSize; ++i) {
    for (auto it = _collumn.begin(); it != _collumn.end(); ++it)
      fstream << std::left << std::setw(20) << it->_values[i] << " ";
    fstream << std::endl;
  }

  fstream.close();
}

void Arrays::Draw(FILE * iPipe) const
{
  WriteToFile(_tmpFileName);
  DrawArrays(iPipe);
}

void Arrays::DrawArrays(FILE * iPipe) const
{
  for (size_t i = 1; i < _collumn.size(); ++i) {
    std::stringstream command;
    command << "replot \"" << _tmpFileName << "\" u 1:" << i + 1 << " w l t '" << _collumn[i]._name.data() << "(x)'\n";
    fprintf(iPipe, command.str().data());
  }
}