#pragma once
#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <map>
#include <string>

/* ************************************************************************** */
class Headers
{
public:
  void addHeader(const std::string& key, const std::string& value);

  std::string toString() const;
  std::string toLogString() const;

  const std::string& operator[](const std::string& key);
  bool contains(const std::string& key) const;

private:
  typedef std::map<std::string, std::string> HeaderMap;

  static void _formatInput(std::string& key, std::string& value);
  static bool _isValidInput(const std::string& key, const std::string& value);
  void _addNew(const std::string& key, const std::string& value);
  void _addExisting(const std::string& key, const std::string& value);

  HeaderMap _headers;
};

#endif
