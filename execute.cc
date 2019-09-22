#include "globals.hh"
#include "TRunManager.hh"

int main(int argc, char** argv)
{
  TRunManager* runManager = new TRunManager;
  runManager -> Run(argc, argv); 

  return 0;
}
