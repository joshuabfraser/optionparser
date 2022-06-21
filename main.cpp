#include <iostream>
#include "OptionParser.h"

using namespace std;

int main(int argc, char *argv[])
{
  OptionParser parser;

  parser.addOption('h', "help", "Display this help.", "", "");
  parser.addOption('\0', "longoptiononly", "", "", "");
  parser.addOption('r', "radius", "Radius of circle to sample", "radius",
                   10.0f);
  parser.addOption('t', "total", "Total number of samples", "samples",
                   8192);
  parser.addOption(0, "stupid", "Stupid flag that only accepts long option.",
                   "string", "");

  bool reallylong = false;
  parser.addOption(0, "reallylong", "Flag for really long options and this description is too long.", "bool",
                   reallylong);
  parser.addOption('s', "", "Short flag only.", "", "");

  parser.parse(argc, argv);

  if(parser.isSet("help"))
  {
    std::cout << parser.helpString();
    exit(EXIT_SUCCESS);
  }

  float radius = 0.0;
  parser.getOptionalValue("radius", &radius);
  std::cout << "Got radius = " << radius << '\n';

  int samples;
  parser.getRequiredValue("total", &samples);

  std::string stupid;
  parser.getOptionalValue("stupid", &stupid);
  std::cout << "stupid = " << stupid << '\n';
  return 0;
}
