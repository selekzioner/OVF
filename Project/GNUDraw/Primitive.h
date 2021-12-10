#pragma once
#include <iostream>
#include <fstream>

class Primitive;
typedef std::shared_ptr<Primitive> PrimitivePtr;

class Primitive
{
public:
  virtual void Draw(FILE*iPipe) const = 0;
};
