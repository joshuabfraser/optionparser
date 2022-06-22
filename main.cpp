#include <iostream>
#include "OptionParser.h"

using namespace std;

int main(int argc, char *argv[])
{
  OptionParser parser;

  int total = 8192;
  parser.addHelpOption();
  parser.addOption('\0', "longoptiononly", "", "", "");
  parser.addOption('r', "radius", "Radius of circle to sample", "radius",
                   10.0f);
  parser.addOption('t', "total", "Total number of samples", "samples",
                   total);
  parser.addOption(0, "stupid", "Stupid flag that only accepts long option.",
                   "string", "");

  bool reallylong = false;
  parser.addOption(0, "reallylong", "Flag for really long options and this description is too long.", "bool",
                   reallylong);
  parser.addOption('s', "", "Short flag only.", "", "");

  parser.addPositionalArgument("input");
  parser.addPositionalArgument("ouptut");

  parser.parse(argc, argv);

  std::cout << "Positional arguments: \n";
  for(const auto& a : parser.arguments())
  {
    std::cout << a << '\n';
  }
  float radius = 0.0;
  parser.getOptionalValue("radius", &radius);
  std::cout << "Got radius = " << radius << '\n';

  std::string stupid;
  parser.getOptionalValue("stupid", &stupid);
  std::cout << "stupid = " << stupid << '\n';

  if(!parser.getOptionalValue("total", &total))
  {
    std::cerr << "Failed converting option \"--total\"\n";
  }
  std::cout << "total = " << total << '\n';

  return 0;
}
