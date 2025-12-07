#include "ValidateHeader.hpp"

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

Logger& ValidateHeader::_log = Logger::getInstance(LOG_HTTP);

const ft::array<ValidateHeader::ValidatorEntry, ValidateHeader::_validators>
  ValidateHeader::_validatorMap = { {
    { "host", &ValidateHeader::_validateHost },
    { "content-length", &ValidateHeader::_validateContentLength },
    { "transfer-encoding", &ValidateHeader::_validateTransferEncoding },
  } };

/* ************************************************************************** */
// PUBLIC

ValidateHeader::ValidateHeader(Client* client)
  : _client(client)
  , _headers(&_client->getRequest().getHeaders())
  , _closeConnection(false)
{
}

void ValidateHeader::run(const std::string& name, const std::string& value)
{
  _closeConnection = false;

  const std::string key = ft::to_lower(ft::trim(name));
  for (std::size_t i = 0; i < _validatorMap.size(); ++i) {
    const ValidatorEntry& entry = _validatorMap[i];
    if (entry.name == key) {
      (this->*entry.fnPtrValid)(value);
      break;
    }
  }

  _setResponseConnectionHeader();
}

/* ************************************************************************** */
// PRIVATE

void ValidateHeader::_validateHost(const std::string& value)
{
  if (_headers->contains("Host")) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ValidateHeader: found duplicate Host header\n";
    return;
  }

  static const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(hostHeaderRule());
  if (!isValidString(*rule, value)) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ValidateHeader: invalid Host header\n";
  }
}

void ValidateHeader::_validateContentLength(const std::string& value)
{
  const bool hasTransferEncoding = _headers->contains("Transfer-Encoding");
  if (hasTransferEncoding) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _closeConnection = true;
    _log.error()
      << "ValidateHeader: has Transfer-Encoding AND Content-Length\n";
    return;
  }

  static const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(contentLengthRule());
  if (!isValidString(*rule, value)) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ValidateHeader: invalid Content-Length header\n";
  }
}

void ValidateHeader::_validateTransferEncoding(const std::string& value)
{
  const bool hasContentLength = _headers->contains("Content-Length");
  if (hasContentLength) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _closeConnection = true;
    _log.error()
      << "ValidateHeader: has Transfer-Encoding AND Content-Length\n";
    return;
  }

  if (_client->getRequest().getVersion() == "HTTP/1.0") {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _closeConnection = true;
    _log.error()
      << "ValidateHeader: has Transfer-Encoding with version HTTP/1.0\n";
    return;
  }

  static const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(transferEncodingRule());
  if (!isValidString(*rule, value)) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.error() << "ValidateHeader: invalid Transfer-Encoding header\n";
  }
}

void ValidateHeader::_setResponseConnectionHeader()
{
  Headers& headers = _client->getResponse().getHeaders();
  if (_closeConnection && !headers.contains("Connection")) {
    headers.addHeader("Connection", "close");
  }
}
