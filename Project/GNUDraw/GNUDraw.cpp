#include "GNUDraw.h"

GNUDrawer::GNUDrawer(const std::string & iName) 
  : _winName(iName)
{
}

void GNUDrawer::Draw() const
{
  FILE* gPipe = PreparePipe();
  PreparePlot(gPipe);
  DrawPrimitives(gPipe);
}

void GNUDrawer::Add(const PrimitivePtr& iPrimitive)
{
  _primitives.push_back(iPrimitive);
}

void GNUDrawer::LogScale(bool iLogScale)
{
  _logScale = iLogScale;
}

void GNUDrawer::DrawPrimitives(FILE * iPipe) const
{
  for (auto it = _primitives.begin(); it != _primitives.end(); ++it) {
    (*it)->Draw(iPipe);
  }
}

FILE * GNUDrawer::PreparePipe() const
{
  FILE *gPipe = _popen("gnuplot.exe -persist", "w");
  if (!gPipe)
    throw "Pipe creation error";
  return gPipe;
}

void GNUDrawer::PreparePlot(FILE * iPipe) const
{
  if (_winName.empty())
    fprintf(iPipe, "set terminal win\n");
  else {
    std::string setTerminal = "set terminal win title '" + _winName + "'\n";
    fprintf(iPipe, setTerminal.data());
  }
  if (_logScale)
  {
    fprintf(iPipe, "set logscale x 2\n");
  }
  fprintf(iPipe, "set grid xtics ytics mxtics mytics\n");
  fprintf(iPipe, "plot 0 notitle\n");
}

void GNUDrawer::ClosePipe(FILE * iPipe) const
{
}
