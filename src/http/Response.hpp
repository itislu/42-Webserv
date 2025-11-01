#pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <http/Headers.hpp>
#include <http/StatusCode.hpp>

#include <fstream>
#include <string>

/* ************************************************************************** */
class Response
{
public:
  const std::string& getVersion() const;
  void setVersion(const std::string& version);

  const StatusCode& getStatusCode() const;
  void setStatusCode(StatusCode::Code code);

  Headers& getHeaders();

  std::ifstream& getInputFileStream();

private:
  std::string _version;
  StatusCode _statuscode;
  Headers _headers;
  std::ifstream _inputFileStream;
};

#endif
