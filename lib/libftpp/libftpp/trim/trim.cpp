#include "trim.hpp"
#include <algorithm>
#include <cctype>
#include <string>

static bool isNoSpace(int chr)
{
  return std::isspace(chr) == 0;
}

std::string& trim(std::string& str)
{
  // trim left
  const std::string::iterator itBegin =
    std::find_if(str.begin(), str.end(), isNoSpace);
  if (itBegin == str.end()) {
    str = std::string("");
    return str;
  }
  str.erase(str.begin(), itBegin);

  // trim right
  const std::string::reverse_iterator itEnd =
    std::find_if(str.rbegin(), str.rend(), isNoSpace);
  str.erase(itEnd.base(), str.end());
  return str;
}
