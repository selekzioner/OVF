#pragma once
#include "Primitive.h"
#include <sstream>

class VerticalLine;

typedef std::shared_ptr<VerticalLine> VerticalLinePtr;

class VerticalLine : public Primitive
{
public:
  static VerticalLinePtr Construct(double iX);

  virtual void Draw(FILE* iPipe) const;

private:
  VerticalLine(double iX);
  double _x;
};