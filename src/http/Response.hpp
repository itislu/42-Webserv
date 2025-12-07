#pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/memory.hpp>
#include <utils/buffer/IInBuffer.hpp>

#include <string>

/* ************************************************************************** */
class Response
{
public:
  const std::string& getVersion() const;
  const StatusCode& getStatusCode() const;
  Headers& getHeaders();
  const ft::shared_ptr<IInBuffer>& getBody() const;

  void setVersion(const std::string& version);
  void setStatusCode(StatusCode::Code code);
  void setBody(ft::shared_ptr<IInBuffer> body);

private:
  std::string _version;
  StatusCode _statuscode;
  Headers _headers;
  ft::shared_ptr<IInBuffer> _body;
};

#endif
