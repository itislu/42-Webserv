#include "RequestHeaderValidator.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/headerRules.hpp>
#include <http/abnfRules/ruleUtils.hpp>
#include <http/headerUtils.hpp>
#include <http/http.hpp>
#include <libftpp/array.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>

#include <cstddef>
#include <string>

/* ***************************************************************************/
// INIT

Logger& RequestHeaderValidator::_log = Logger::getInstance(LOG_HTTP);

const ft::array<RequestHeaderValidator::ValidatorEntry,
                RequestHeaderValidator::_validators>
  RequestHeaderValidator::_validatorMap = { {
    { "host", &RequestHeaderValidator::_validateHost },
    { "content-length", &RequestHeaderValidator::_validateContentLength },
    { "transfer-encoding", &RequestHeaderValidator::_validateTransferEncoding },
  } };

/* ************************************************************************** */
// PUBLIC

RequestHeaderValidator::RequestHeaderValidator(Client* client)
  : _client(client)
  , _headers(&_client->getRequest().getHeaders())
{
}

bool RequestHeaderValidator::isValid(const std::string& name,
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

/**
 * https://datatracker.ietf.org/doc/html/rfc9112#section-3.2-6
 */
void RequestHeaderValidator::_validateHost(const std::string& value)
{
  if (_headers->contains("Host")) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "RequestHeaderValidator: found duplicate Host header\n";
    return;
  }

  static const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(hostHeaderRule());
  if (!isValidString(*rule, value)) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "RequestHeaderValidator: invalid Host header\n";
  }
}

/**
 * https://datatracker.ietf.org/doc/html/rfc9112#section-6.1-15
 */
void RequestHeaderValidator::_validateContentLength(const std::string& value)
{
  const bool hasTransferEncoding = _headers->contains(header::transferEncoding);
  if (hasTransferEncoding) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _client->setCloseConnection(true);
    _log.error()
      << "RequestHeaderValidator: has Transfer-Encoding AND Content-Length\n";
    return;
  }

  static const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(contentLengthRule());
  if (!isValidString(*rule, value)) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "RequestHeaderValidator: invalid Content-Length header\n";
  }
}

/**
 * https://datatracker.ietf.org/doc/html/rfc9112#section-6.1-15
 * https://datatracker.ietf.org/doc/html/rfc9112#section-6.1-16
 */
void RequestHeaderValidator::_validateTransferEncoding(const std::string& value)
{
  const bool hasContentLength = _headers->contains("Content-Length");
  if (hasContentLength) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _client->setCloseConnection(true);
    _log.error()
      << "RequestHeaderValidator: has Transfer-Encoding AND Content-Length\n";
    return;
  }

  if (_client->getRequest().getVersion() == http::HTTP_1_0) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _client->setCloseConnection(true);
    _log.error() << "RequestHeaderValidator: has Transfer-Encoding with "
                    "version HTTP/1.0\n";
    return;
  }

  static const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(transferEncodingRule());
  if (!isValidString(*rule, value)) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error()
      << "RequestHeaderValidator: invalid Transfer-Encoding header\n";
  }
}
