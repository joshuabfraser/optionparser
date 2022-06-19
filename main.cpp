#include <iostream>
#include "OptionParser.h"

using namespace std;

int main(int argc, char *argv[])
{
  OptionParser parser;

  parser.addOption('r', "radius", "Radius of circle to sample", "radius",
                   10.0f);
  parser.addOption('t', "total", "Total number of samples", "samples",
                   8192);

  parser.addOption(0, "stupid", "Stupid flag that only accepts long option.",
                   "string", "");

  bool reallylong = false;

  parser.addOption(0, "reallylong", "Flag for really long options.", "bool",
                   reallylong);
  parser.addOption('s', "", "Short flag only.", "", "");

  parser.parse(argc, argv);

  std::cout << parser.helpString();

  return 0;
}