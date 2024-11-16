#include <OptionParser.h>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
  // Initialize with program name that will be shown in the help
  OptionParser parser("example");

  // Include the -h, --help option
  parser.addHelpOption();

  // Command line option that accepts -i, --input
  parser.addOption('i', "input", "Path for input.", "file");

  float radius = 1.0f;

  // Command line option that accepts -r, --radius and provides a default value
  parser.addOption('r', "radius", "Search radius.", "float", radius);

  // Command line bool option that accepts -x. --exclude with default value
  // Boolean flags accepts these values for true: true, on, yes, 1
  // and these for false: false, off, no, 0
  bool exclude = false;
  parser.addOption('x', "exclude", "Prune outliers.", "bool", exclude);

  // Command line option with no values
  parser.addOption('s', "skip", "Skip pre-processing.");

  // Command line option that accepts -v, --vector for a range of values
  // Values can be comma delimited or an inclusive range with an ellipsis,
  // e.g. 1,2,3,4,5 is the same as 1...5
  // The inclusive range is incremented by 1
  parser.addOption('v', "vector", "Add a range to a vector.", "range");

  // Adds this value as a positional argument in the help
  parser.addPositionalArgument("output");

  // Parse the command line
  parser.parse(argc, argv);

  // Get required options
  std::string input;
  parser.getRequiredValue("input", &input);

  // Get optional parameters
  parser.getOptionalValue("exclude", &exclude);
  parser.getOptionalValue("radius", &radius);

  // Get optional range
  std::vector<int> vec;
  parser.getOptionalRange("vector", vec);

  // Test if option has been specified on the command line
  if(parser.isSet("skip"))
  {
    // Don't do pre-processing
    std::cout << "Skipping pre-processing.\n";
  }

  // Get list of positional arguments
  auto positional = parser.arguments();

  std::string output;
  if(positional.size() == 1)
  {
    output = positional.at(0);
  } else
  {
    std::cerr << "Exactly one positional argument required for output.\n"
              << parser.helpString();
  }

  std::cout << "Parsed arguments:\n";
  std::cout << "radius: " << radius << '\n';
  std::cout << "input:  " << input << '\n';
  std::cout << "exclude: " << exclude << '\n';
  std::cout << "output:  " << output << '\n';
  std::cout << "vector: ";
  for(auto v : vec)
    std::cout << v << ' ';
  std::cout << '\n';

  return 0;
}
