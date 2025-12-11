#include "CgiHeaderValidator.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/headerRules.hpp>
#include <http/abnfRules/ruleUtils.hpp>
#include <libftpp/array.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>

#include <cstddef>
#include <string>

/* ***************************************************************************/
// INIT

Logger& CgiHeaderValidator::_log = Logger::getInstance(LOG_HTTP);

const ft::array<CgiHeaderValidator::ValidatorEntry,
                CgiHeaderValidator::_validators>
  CgiHeaderValidator::_validatorMap = { {
    { "content-length", &CgiHeaderValidator::_validateContentLength },
    { "transfer-encoding", &CgiHeaderValidator::_validateTransferEncoding },
  } };

/* ************************************************************************** */
// PUBLIC

CgiHeaderValidator::CgiHeaderValidator(Client* client)
  : _client(client)
  , _headers(&_client->getResponse().getHeaders())
{
}

// todo check returned statuscodes

bool CgiHeaderValidator::isValid(const std::string& name,
                                 const std::string& value)
{
  const std::string key = ft::to_lower(ft::trim(name));
  for (std::size_t i = 0; i < _validatorMap.size(); ++i) {
    const ValidatorEntry& entry = _validatorMap[i];
    if (entry.name == key) {
      (this->*entry.fnPtrValid)(value);
      break;
    }
  }

  return _client->getResponse().getStatusCode() == StatusCode::Ok;
}

/* ************************************************************************** */
// PRIVATE

void CgiHeaderValidator::_validateContentLength(const std::string& value)
{
  const bool hasTransferEncoding = _headers->contains("Transfer-Encoding");
  if (hasTransferEncoding) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error()
      << "CgiHeaderValidator: has Transfer-Encoding AND Content-Length\n";
    return;
  }

  static const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(contentLengthRule());
  if (!isValidString(*rule, value)) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "CgiHeaderValidator: invalid Content-Length header\n";
  }
}

void CgiHeaderValidator::_validateTransferEncoding(const std::string& value)
{
  const bool hasContentLength = _headers->contains("Content-Length");
  if (hasContentLength) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error()
      << "CgiHeaderValidator: has Transfer-Encoding AND Content-Length\n";
    return;
  }

  if (_client->getRequest().getVersion() == "HTTP/1.0") {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "CgiHeaderValidator: has Transfer-Encoding with "
                    "version HTTP/1.0\n";
    return;
  }

  static const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(transferEncodingRule());
  if (!isValidString(*rule, value)) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "CgiHeaderValidator: invalid Transfer-Encoding header\n";
  }
}
