#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H
#include <functional>
#include <getopt.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class OptionParser
{
public:
  OptionParser(const std::string &programName = "");

  void setErrorFunction(std::function<void (const std::string &)> error)
  {
    m_error = error;
  }

  template<typename T>
  bool addOption(char flag, const std::string &name,
                 const std::string &description, const std::string &valueName,
                 const T &defaultValue)
  {
    std::stringstream ss;
    ss << defaultValue;

    if(!ss)
    {
      m_error("Unable to string convert default value for option "
                + name + '\n');
      return false;
    }

    m_options.push_back({flag, name, description, valueName, ss.str(), ""});

    return true;
  }

  bool addOption(char flag, const std::string &name,
                 const std::string &description, const std::string &valueName,
                 const bool &defaultValue)
  {
    return addOption(flag, name, description, valueName,
                     defaultValue ? "true" : "false");
  }

  // Specialization to force decimal point when formatting doubles
  bool addOption(char flag, const std::string &name,
                 const std::string &description, const std::string &valueName,
                 const double &defaultValue)
  {
    std::stringstream stream;
    // Force decimal point for floating precision values
    if(defaultValue == (int)defaultValue)
      stream << std::fixed << std::setprecision(1);

    stream << defaultValue;

    return addOption(flag, name, description, valueName, stream.str());
  }

  // Specialization to force decimal point when formatting floats
  bool addOption(char flag, const std::string &name,
                 const std::string &description, const std::string &valueName,
                 const float &defaultValue)
  {
    return addOption(flag, name, description, valueName, (double) defaultValue);
  }

  bool addOption(char flag, const std::string &name,
                 const std::string &description)
  {
    return addOption(flag, name, description, "", "");
  }

  void addPositionalArgument(const std::string& arg)
  {
    m_positional.push_back(arg);
  }

  void addHelpOption(const std::string& message = "Display help message.")
  {
    m_hasHelp = true;
    addOption('h', "help", message);
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
    if(!getOptionValueString(name, value)) return false;

    return !(value->empty());
  }

  bool getOptionalValue(const std::string &name, bool *value) const
  {
    std::string s;

    // Get string returned from parsing
    if(!getOptionValueString(name, &s)) return false;

    // Check string equivalents for booleans
    if(s == "true"  || s == "on"  || s == "yes" || s == "1")
    {
      *value = true;
      return true;
    }

    if(s == "false" || s == "off" || s == "no"  || s == "0")
    {
      *value = false;
      return true;
    }

    m_error("Failed to convert boolean value for option --" + name);

    return false;
  }

  template<typename T>
  bool getOptionalValue(const std::string &name, T *value) const
  {
    std::string result;

    // Get string returned from parsing
    if(!getOptionValueString(name, &result)) return false;

    // Stream convert string to type T
    std::stringstream stream(result);
    stream >> *value;

    // Success if stream is ok and entire string converted; note good() and
    // eof() are mutually exclusive.
    bool success = stream && stream.eof();
    if(!success) m_error("Failed to convert option --" + name);

    return success;
  }

  template<typename T>
  bool getRequiredValue(const std::string &name, T *value) const
  {
    if(!isSet(name))
    {
      m_error("Missing required option " + name);
    }

    return getOptionalValue(name, value);
  }

  const std::vector<std::string>& arguments() const { return m_arguments; }

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

  bool getOptionValueString(const std::string& name, std::string* value) const;

  std::string formatFlags(const struct longoption& option) const;

  std::vector<longoption> m_options;
  std::string m_programName;
  bool m_hasHelp = false;

  std::vector<std::string> m_positional;
  std::vector<std::string> m_arguments;

  std::function<void (const std::string &)> m_error = [](const std::string& s)
  {
    std::cerr << s << '\n';
    exit(EXIT_FAILURE);
  };

  int m_maxColumn = 80;
};

#endif // OPTIONPARSER_H
