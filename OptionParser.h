#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H
#include <getopt.h>
#include <sstream>
#include <string>
#include <vector>

class OptionParser
{
public:
  OptionParser();

//  void addOption(char flag, const std::string& name,
//                 const std::string& description, const std::string& valueName,
//                 const std::string& defaultValue);

  template<typename T>
  void addOption(char flag, const std::string &name,
                 const std::string &description, const std::string &valueName,
                 const T &defaultValue)
  {
    std::stringstream ss;
    ss << defaultValue;

    m_options.push_back({flag, name, description, valueName, ss.str()});
  }

  bool parse(int argc, char *argv[]);

  std::string helpString() const;

private:

  struct longoption {
    char flag;
    std::string name;
    std::string description;
    std::string value;
    std::string defaultValue;
  };

  std::string formatFlags(const struct longoption& option) const;

  std::vector<longoption> m_options;
  std::string m_programName;
};

#endif // OPTIONPARSER_H
