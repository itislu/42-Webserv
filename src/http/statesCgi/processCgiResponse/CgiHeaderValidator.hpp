#pragma once
#ifndef REQUEST_HEADER_VALIDATOR_HPP
#define REQUEST_HEADER_VALIDATOR_HPP

#include <http/Headers.hpp>
#include <http/utils/BaseHeaderValidator.hpp>
#include <libftpp/array.hpp>

#include <string>

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class CgiHeaderValidator : public BaseHeaderValidator
{
public:
  explicit CgiHeaderValidator(Client* client);
  bool isValid(const std::string& name, const std::string& value);

private:
  struct ValidatorEntry;
  typedef void (CgiHeaderValidator::*fnPtrValid)(const std::string&);
  static const int _validators = 2;
  static const ft::array<ValidatorEntry, _validators> _validatorMap;

  void _validateContentLength(const std::string& value);
  void _validateTransferEncoding(const std::string& value);

  static Logger& _log;
  Client* _client;
  Headers* _headers;
};

struct CgiHeaderValidator::ValidatorEntry
{
  const char* name;
  CgiHeaderValidator::fnPtrValid fnPtrValid;
};

#endif
