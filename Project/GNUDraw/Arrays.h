#pragma once
#include "Primitive.h"
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>

class Arrays;
typedef std::shared_ptr<Arrays> ArraysPtr;

class Collumn
{
public:
  Collumn(const std::string& iName, const std::vector<double>& iValues);
  size_t Size() const;

  friend class Arrays;
private:
  std::vector<double> _values = {};
  std::string _name;
};

class Arrays : public Primitive
{
public:
  static ArraysPtr Construct();

  void Add(const std::string& iName, const std::vector<double>& iValues);
  void WriteToFile(const std::string& iFile) const;
  virtual void Draw(FILE* iPipe) const;

  static size_t _arraySize;
private:
  Arrays();
  void DrawArrays(FILE* iPipe) const;

  std::string _tmpFileName;
  size_t _collumnSize = 0;
  std::vector<Collumn> _collumn = {};
};
