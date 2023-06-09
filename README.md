# OptionParser

OptionParser is a `getopt_long()` wrapper library for parsing command-line 
options in C++ for Unix-like platforms.

## Getting Started

### Requirements

- C++ compiler supporting C++11.
- CMake version 3.10 or above.
- Platform support for GNU or BSD `getopt_long()`

### Including library in CMake projects

OptionParser is meant to be included in CMake projects using `FetchContent`:

```cmake
project(foo LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  OptionParser
  GIT_REPOSITORY https://github.com/joshuabfraser/options
  GIT_TAG main
)

FetchContent_MakeAvailable(OptionParser)
target_link_libraries (foo PRIVATE OptionParser)
```

### Parsing command line options

See `example.cpp` below.  It produces formatted output for help:

```
usage: example [options] output

Options:
  -h, --help            Display help message.                                   
  -i, --input <file>    Path for input.                                         
  -r, --radius <float>  Search radius. (Default: 1.0)                           
  -x, --exclude <bool>  Prune outliers. (Default: false)                        
  -s, --skip            Skip pre-processing.     
```

```c++
#include <OptionParser.h>
#include <iostream>
#include <string>

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
    std::cerr << "Exactly on positional argument required for output.\n"
              << parser.helpString();
  }

  std::cout << "Parsed arguments:\n";
  std::cout << "radius: " << radius << '\n';
  std::cout << "input:  " << input << '\n';
  std::cout << "exclude: " << exclude << '\n';
  std::cout << "output:  " << output << '\n';

  return 0;
}
```

Copyright 2022-2023, Joshua Fraser  
