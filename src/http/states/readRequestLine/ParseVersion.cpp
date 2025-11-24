#include "ParseVersion.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <http/states/validateRequest/ValidateRequest.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <ctype.h>
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
  _log.info() << "ParseVersion\n";
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
    getContext()->getStateHandler().setDone();
    return;
  }
}

void ParseVersion::_extractVersion()
{
  const long index = _buffReader.getPosInBuff();
  std::string input = _client->getInBuff().consumeFront(index);
  ft::trim(input);
  _client->getRequest().setVersion(input);
}

/* ************************************************************************** */
// PRIVATE

void ParseVersion::_init()
{
  _buffReader.init(&_client->getInBuff());

  // _sequence.addRule(owsRule());
  _sequence.addRule(ft::make_shared<LiteralRule>("HTTP/"));
  _sequence.addRule(ft::make_shared<RangeRule>(::isdigit));
  _sequence.addRule(ft::make_shared<LiteralRule>("."));
  _sequence.addRule(ft::make_shared<RangeRule>(::isdigit));
  _sequence.addRule(endOfLineRule());
  _sequence.setBufferReader(&_buffReader);
}
