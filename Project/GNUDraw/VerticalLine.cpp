#include "VerticalLine.h"

VerticalLine::VerticalLine(double iX)
  : _x(iX)
{
}

VerticalLinePtr VerticalLine::Construct(double iX)
{
  return VerticalLinePtr(new VerticalLine(iX));
}

void VerticalLine::Draw(FILE * iPipe) const
{
  std::stringstream command;
  command << "set arrow from " << _x << ",-10000 to " << _x << ",10000 nohead \n";
  fprintf(iPipe, command.str().data());
}