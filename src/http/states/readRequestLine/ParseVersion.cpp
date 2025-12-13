#include "ParseVersion.hpp"

#include <client/Client.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/http.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/ctype.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
// INIT

Logger& ParseVersion::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ParseVersion::ParseVersion(ReadRequestLine* context)
  : IState<ReadRequestLine>(context)
  , _client(context->getContext())
  , _buffReader()
{
  _log.info() << *_client << " ParseVersion\n";
  _init();
}

/* ************************************************************************** */
// PRIVATE

/**
 * @brief Parse version from first line
 *
 * 'HTTP/1.0'
 *
 * HTTP-version  = HTTP-name "/" DIGIT "." DIGIT
 * HTTP-name     = %s"HTTP"
 */
void ParseVersion::run()
{
  _sequence.reset();
  _buffReader.resetPosInBuff();
  if (!_sequence.matches()) {
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _log.info() << "ParseVersion: no match\n";
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_sequence.reachedEnd()) {
    _extractVersion();
    _validateVersion();
    getContext()->getStateHandler().setDone();
    return;
  }
}

/**
 * A recipient that receives a message with a major version number that it
 * implements and a minor version number higher than what it implements SHOULD
 * process the message as if it were in the highest minor version within that
 * major version to which the recipient is conformant.
 *
 * https://datatracker.ietf.org/doc/html/rfc9110#section-6.2-8
 */
void ParseVersion::_extractVersion()
{
  const std::size_t index = _buffReader.getPosInBuff();
  std::string version = _client->getInBuff().consumeFront(index);
  ft::trim(version);
  if (version > http::HTTP_1_1 && version < http::HTTP_2_0) {
    version = http::HTTP_1_1;
  }
  _client->getRequest().setVersion(version);
}

/* ************************************************************************** */
// PRIVATE

void ParseVersion::_init()
{
  _buffReader.init(&_client->getInBuff());

  // _sequence.addRule(owsRule());
  _sequence.addRule(ft::make_shared<LiteralRule>("HTTP/"));
  _sequence.addRule(ft::make_shared<RangeRule>(ft::isdigit));
  _sequence.addRule(ft::make_shared<LiteralRule>("."));
  _sequence.addRule(ft::make_shared<RangeRule>(ft::isdigit));
  _sequence.addRule(endOfLineRule());
  _sequence.setBufferReader(&_buffReader);
}

/**
 * A server can send a 505 (HTTP Version Not Supported) response if it wishes,
 * for any reason, to refuse service of the client's major protocol version.
 * https://datatracker.ietf.org/doc/html/rfc9110#section-6.2-7
 *
 * The 505 (HTTP Version Not Supported) status code indicates that the server
 * does not support, or refuses to support, the major version of HTTP that was
 * used in the request message.
 * https://datatracker.ietf.org/doc/html/rfc9110#name-505-http-version-not-suppor
 */
void ParseVersion::_validateVersion()
{
  Response& response = _client->getResponse();
  const std::string& version = _client->getRequest().getVersion();
  if (!ft::starts_with(version, "HTTP/1.")) {
    response.setStatusCode(StatusCode::HttpVersionNotSupported);
  }
}
