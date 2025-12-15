#pragma once
#ifndef AUTHORITY_HPP
#define AUTHORITY_HPP

#include <string>

class Authority
{
public:
  const std::string& getUserinfo() const;
  const std::string& getHost() const;
  const std::string& getPort() const;

  void setUserinfo(const std::string& userinfo);
  void setHost(const std::string& host);
  void setPort(const std::string& port);

  std::string toString() const;

private:
  std::string _userinfo;
  std::string _host;
  std::string _port;
};

#endif
