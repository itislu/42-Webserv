#pragma once
#ifndef VALIDATE_HEADER_HPP
#define VALIDATE_HEADER_HPP

#include <http/Headers.hpp>
#include <libftpp/array.hpp>

#include <string>

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class ValidateHeader
{
public:
  explicit ValidateHeader(Client* client);
  void run(const std::string& name, const std::string& value);

private:
  struct ValidatorEntry;
  typedef void (ValidateHeader::*fnPtrValid)(const std::string&);
  static const int _validators = 3;
  static const ft::array<ValidatorEntry, _validators> _validatorMap;

  void _validateHost(const std::string& value);
  void _validateContentLength(const std::string& value);
  void _validateTransferEncoding(const std::string& value);

  void _setResponseConnectionHeader();

  static Logger& _log;
  Client* _client;
  Headers* _headers;

  bool _closeConnection;
};

struct ValidateHeader::ValidatorEntry
{
  const char* name;
  ValidateHeader::fnPtrValid fnPtrValid;
};

#endif
