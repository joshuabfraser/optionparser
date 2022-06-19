#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "OptionParser.h"

OptionParser::OptionParser()
{

}

//void OptionParser::addOption(char flag, const std::string &name,
//                             const std::string &description,
//                             const std::string &valueName,
//                             const std::string &defaultValue)
//{
//  longoption opt{flag, name, description, valueName, defaultValue};
//  m_options.push_back({flag, name, description, valueName, defaultValue});
//}

bool OptionParser::parse(int argc, char *argv[])
{
  // Get program name if not set
  if(m_programName.empty())
  {
    m_programName = argv[0];
    m_programName = m_programName.substr(m_programName.find_last_of("/\\") + 1);
  }

  std::vector<struct option> options;
  std::string flags = ":";

  int i = 0;
  for(const auto& o : m_options)
  {
    options.push_back({ o.name.c_str(), required_argument, 0, o.flag });

    if(o.flag)
      flags += o.flag + std::string(":");
  }

  std::cout << "flags: " << flags << '\n';

  int index = 0;
  int o;
  while((o = getopt_long(argc, argv, &flags[0], &options[0], &index)) != -1)
  {
    std::cout << "o = " << char(o) << '\n';
    std::cout << "Got index " << index << "\n";

    if(optarg)
      std::cout << "Got argument " << optarg << '\n';
  }

  return true;
}

std::string OptionParser::formatFlags(const struct longoption& option) const
{
  std::stringstream flagStream;

  flagStream << "  ";

  if(option.flag)
    flagStream << "-" << option.flag << ", ";
  else
    flagStream << "    ";

  if(!option.name.empty())
      flagStream << "--" << option.name;

  if(!option.value.empty())
    flagStream << " <" << option.value << ">  ";

  return flagStream.str();
}

std::string OptionParser::helpString() const
{

  std::stringstream stream;

  stream << "usage: " <<  m_programName << " [options]" << std::endl
         << std::endl
         << "Options:" << std::endl;

  // Build flags column and determine max width
  unsigned long flagWidth = 0;
  std::vector<std::string> flagText;
  for(const auto& o : m_options)
  {
    flagWidth = std::max(flagWidth, formatFlags(o).length());
  }

  for(const auto& o : m_options)
  {
    stream << std::left << std::setw(flagWidth) << formatFlags(o);
    stream << std::left << std::setw(80 - flagWidth) << o.description
           << std::endl;
  }

  return stream.str();
}
