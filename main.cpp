#include <iostream>
#include "OptionParser.h"

int main(int argc, char *argv[])
{
  OptionParser parser("optionParser");

  int total = 8192;
  bool reallylong = false;
  float radius = 10.0f;

  parser.addHelpOption();
  parser.addOption('\0', "longoptiononly", "", "", "");
  parser.addOption('r', "radius", "Radius of circle to sample", "radius",
                   radius);
  parser.addOption('t', "total", "Total number of samples", "samples",
                   total);
  parser.addOption(0, "stupid", "Stupid flag that only accepts long option.",
                   "string", "");
  parser.addOption(0, "reallylong", "Flag for really long options and this description is too long.", "bool",
                   reallylong);
  parser.addOption('s', "", "Short flag only.", "", "");

  parser.addPositionalArgument("input");
  parser.addPositionalArgument("ouptut");

  parser.parse(argc, argv);

  parser.getOptionalValue("radius", &radius);
  std::cout << "Got radius = " << radius << '\n';

  std::string stupid;
  parser.getOptionalValue("stupid", &stupid);
  std::cout << "stupid = " << stupid << '\n';

  parser.getOptionalValue("total", &total);
  std::cout << "total = " << total << '\n';

  parser.getOptionalValue("reallylong", &reallylong);
  std::cout << "Really long: " << reallylong << '\n';

  std::cout << "Positional arguments: \n";
  for(const auto& a : parser.arguments())
  {
    std::cout << a << '\n';
  }

  return 0;
}
