#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "Primitive.h"


class GNUDrawer
{
public:
  GNUDrawer() = default;
  GNUDrawer(const std::string& iName);
  void Draw() const;
  void Add(const PrimitivePtr& iPrimitive);
  void LogScale(bool iLogScale);
private:
  void DrawPrimitives(FILE* iPipe) const;

  FILE* PreparePipe() const;
  void PreparePlot(FILE* iPipe) const;
  void ClosePipe(FILE* iPipe) const;

  bool _logScale = false;

  std::string _winName;
  std::vector<PrimitivePtr> _primitives = {};
};