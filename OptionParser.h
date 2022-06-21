#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H
#include <getopt.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class OptionParser
{
public:
  OptionParser();

  template<typename T>
  bool addOption(char flag, const std::string &name,
                 const std::string &description, const std::string &valueName,
                 const T &defaultValue)
  {
    std::stringstream ss;
    ss << defaultValue;

    if(!ss)
    {
      std::cerr << "Unable to string convert default value for option "
                << name << '\n';
      return false;
    }

    m_options.push_back({flag, name, description, valueName, ss.str(), ""});

    return true;
  }

  void addHelpOption()
  {
    m_hasHelp = true;
  }

  bool parse(int argc, char *argv[]);

  bool isSet(const std::string& name) const
  {
    int found = getOptionIndex('\0', name);
    return found >= 0 && !m_options.at(found).result.empty();
  }

  std::string helpString() const;

  // Template specialization to read full string including spaces
  bool getOptionalValue(const std::string &name, std::string *value) const
  {
    int found = getOptionIndex('\0', name);

    if(found < 0) return false;

    auto option = m_options.at(found);

    if(option.result.empty() && option.defaultValue.empty()) return false;

    *value = option.result.empty() ? option.defaultValue : option.result;

    return !(value->empty());
  }

  template<typename T>
  bool getOptionalValue(const std::string &name, T *value) const
  {
    int found = getOptionIndex('\0', name);

    if(found < 0) return false;

    auto option = m_options.at(found);

    if(option.result.empty() && option.defaultValue.empty()) return false;

    std::stringstream ss(option.result.empty() ? option.defaultValue : option.result);
    ss >> *value;

    return ss.good();
  }

  template<typename T>
  bool getRequiredValue(const std::string &name, T *value) const
  {
    if(!isSet(name))
    {
      std::cerr << "Missing required option " << name << '\n';
      exit(EXIT_FAILURE);
    }

    return getOptionalValue(name, value);
  }

private:

  struct longoption {
    char flag;
    std::string name;
    std::string description;
    std::string value;
    std::string defaultValue;
    std::string result;
  };

  int getOptionIndex(char o, const std::string& option) const;

  std::string formatFlags(const struct longoption& option) const;

  std::vector<longoption> m_options;
  std::string m_programName;
  bool m_hasHelp = false;

};

#endif // OPTIONPARSER_H
