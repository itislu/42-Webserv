#pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <http/StatusCode.hpp>

#include <string>

/* ************************************************************************** */
class Response
{
public:
  const std::string& getVersion() const;
  void setVersion(const std::string& version);

  StatusCode& getStatusCode();
  void setStatusCode(StatusCode::Code code);

private:
  std::string _version;
  StatusCode _statuscode;
};

#endif
