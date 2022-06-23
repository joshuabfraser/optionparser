#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "OptionParser.h"

OptionParser::OptionParser(const std::string& programName)
  : m_programName(programName)
{
}

bool OptionParser::parse(int argc, char *argv[])
{
  // Get program name if not set
  if(m_programName.empty())
  {
    m_programName = argv[0];
    m_programName = m_programName.substr(m_programName.find_last_of("/\\") + 1);
  }

  std::vector<struct option> options;
  std::string flags = "";

  // Construct optstring and longopts array for getopt_long()
  for(const auto& o : m_options)
  {
    if(o.flag)
      flags += o.flag;

    if(o.value.empty())
    {
      options.push_back({ o.name.c_str(), no_argument, 0, o.flag });
    } else {
      options.push_back({ o.name.c_str(), required_argument, 0, o.flag });
      flags += ':';
    }
  }

  // Terminate longopts with all zeros
  options.push_back({0, 0, 0, 0});

  int o, index = -1;
  while((o = getopt_long(argc, argv, &flags[0], &options[0], &index)) != -1)
  {
    if(o == '?')
      return false;

    std::string longname = index < 0 ? "" : options[index].name;

    int found = getOptionIndex(o, longname);

    if(found < 0) return false;

    if(optarg)
    {
      m_options[found].result = std::string(optarg);
    } else {
      m_options[found].result = std::string("true");
    }

    // getopt_long() sets index to offset in long options for long-only option;
    // reset to -1 to detect when long-only option is given
    index = -1;
  }

  m_arguments = std::vector<std::string>(argv + optind, argv + optind + (argc - optind));

  if(m_hasHelp && isSet("help"))
  {
    std::cout << helpString();
    exit(EXIT_SUCCESS);
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

std::vector<std::string> linewrap(const std::string& text, int max)
{
  std::vector<std::string> result;

  std::string tmp = text;
  while(tmp.length() > max)
  {
    // Find reverse find the nearest space near max
    size_t space =  tmp.rfind(' ', max);
    result.push_back(tmp.substr(0, space));
    tmp.erase(0, space + 1);
  }
  result.push_back(tmp);

  return result;
}

std::string OptionParser::helpString() const
{

  std::stringstream stream;

  stream << "usage: " <<  m_programName << " [options]";
  for(const auto& a : m_positional)
    stream << ' ' << a;
  stream << std::endl
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
    std::string defaultString ;
    if(!o.defaultValue.empty())
      defaultString = " (Default: " + o.defaultValue + ")";

    auto wrapped = linewrap(o.description + defaultString, m_maxColumn - flagWidth);
    stream << std::left << std::setw(flagWidth) << formatFlags(o);
    stream << wrapped.front() << std::endl;
    for(int i = 1; i < wrapped.size(); ++i)
      stream << std::string(flagWidth, ' ')
             << std::left << std::setw(m_maxColumn - flagWidth)
             << wrapped.at(i) << std::endl;
  }

  return stream.str();
}

int OptionParser::getOptionIndex(char flag, const std::string &name) const
{
  for(int i = 0; i < m_options.size(); ++i)
  {
    if(m_options[i].name == name) return i;
    if(flag != '\0' && m_options[i].flag == flag) return i;
  }
  return -1;
}

bool OptionParser::getOptionValueString(const std::string &name,
                                        std::string *value) const
{
  int index = getOptionIndex('\0', name);

  if(index < 0) return false;

  // Favor value set on command line
  *value = m_options.at(index).result;
  // If not set from command line, use default (if provided)
  if(value->empty()) *value = m_options.at(index).defaultValue;

  return true;
}
