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

  const StatusCode& getStatusCode() const;
  void setStatusCode(StatusCode::Code code);

private:
  std::string _version;
  StatusCode _statuscode;
};

#endif
