#include "ParseVersion.hpp"
#include "utils/abnfRules/LiteralRule.hpp"
#include "utils/abnfRules/RangeRule.hpp"
#include "http/abnfRules/requestLineRules.hpp"

#include <cctype>
#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <libftpp/string.hpp>
#include <string>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

ParseVersion::ParseVersion(ReadStartLine* context)
  : IState<ReadStartLine>(context)
  , _client(context->getContext())
  , _buffReader()
  , _init(true)
{
  _buffReader.init(&_client->getInBuff());
}

ParseVersion::~ParseVersion() {}

/* ************************************************************************** */
// PRIVATE

ParseVersion::ParseVersion()
  : IState<ReadStartLine>(NULL)
  , _client(NULL)
  , _buffReader()
  , _init(true)
{
}

ParseVersion::ParseVersion(const ParseVersion& other)
  : IState<ReadStartLine>(other.getContext())
  , _client(other._client)
  , _buffReader()
  , _init(true)
{
  *this = other;
}

ParseVersion& ParseVersion::operator=(const ParseVersion& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

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
  if (_init) {
    _init = false;
    _sequenze.addRule(whitespaceRule(0, 1));
    _sequenze.addRule(new LiteralRule("HTTP/"));
    _sequenze.addRule(new RangeRule(::isdigit));
    _sequenze.addRule(new LiteralRule("."));
    _sequenze.addRule(new RangeRule(::isdigit));
    _sequenze.addRule(endOfRequestLineRule());
    _sequenze.setBufferReader(&_buffReader);
  }

  if (!_sequenze.matches()) {
    // TODO ALARM
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_sequenze.end()) {
    _extractVersion();
    getContext()->getStateHandler().setDone();
    return;
  }
}

void ParseVersion::_extractVersion()
{
  const long bytes = _buffReader.getPosInBuff();
  std::string str = _client->getInBuff().consume(bytes);
  ft::trim(str);
  _client->getRequest().setVersion(str);
}
