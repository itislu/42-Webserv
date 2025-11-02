#include "ParseVersion.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/string.hpp>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/state/IState.hpp>

#include <ctype.h>
#include <string>

/* ************************************************************************** */
// PUBLIC

ParseVersion::ParseVersion(ReadRequestLine* context)
  : IState<ReadRequestLine>(context)
  , _client(context->getContext())
  , _buffReader()
{
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
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_sequence.end()) {
    _extractVersion();
    getContext()->getStateHandler().setDone();
    return;
  }
}

void ParseVersion::_extractVersion()
{
  const long index = _buffReader.getPosInBuff();
  std::string str = _client->getInBuff().consume(index + 1);
  ft::trim(str);
  _client->getRequest().setVersion(str);
}

/* ************************************************************************** */
// PRIVATE

void ParseVersion::_init()
{
  _buffReader.init(&_client->getInBuff());

  _sequence.addRule(rwsRule());
  _sequence.addRule(new LiteralRule("HTTP/"));
  _sequence.addRule(new RangeRule(::isdigit));
  _sequence.addRule(new LiteralRule("."));
  _sequence.addRule(new RangeRule(::isdigit));
  _sequence.addRule(endOfLineRule());
  _sequence.setBufferReader(&_buffReader);
}
