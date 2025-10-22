#include "ParseUri.hpp"

#include "http/abnfRules/requestLineRules.hpp"
#include "http/abnfRules/ruleIds.hpp"
#include "utils/abnfRules/Rule.hpp"
#include "utils/abnfRules/RuleResult.hpp"
#include "utils/abnfRules/SequenceRule.hpp"
#include <client/Client.hpp>
#include <http/Uri.hpp>
#include <http/states/readStartLine/ParseVersion.hpp>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <libftpp/string.hpp>
#include <stdexcept>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
#include <utils/state/IState.hpp>

#include <cctype>
#include <cstddef>
#include <string>

/* ************************************************************************** */
// PUBLIC

ParseUri::ParseUri(ReadStartLine* context)
  : IState<ReadStartLine>(context)
  , _client(context->getContext())
  , _parseState(ParseScheme)
  , _buffReader()
  , _sequence()
  , _initParser(true)
{
  _buffReader.init(&_client->getInBuff());
}

ParseUri::~ParseUri() {}

/* ************************************************************************** */
// PRIVATE

ParseUri::ParseUri()
  : IState<ReadStartLine>(NULL)
  , _client(NULL)
  , _parseState(ParseScheme)
  , _buffReader()
  , _sequence()
  , _initParser(true)
{
}

ParseUri::ParseUri(const ParseUri& other)
  : IState<ReadStartLine>(other.getContext())
  , _client(NULL)
  , _parseState(ParseScheme)
  , _buffReader()
  , _sequence()
  , _initParser(true)
{
  *this = other;
}

ParseUri& ParseUri::operator=(const ParseUri& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

void ParseUri::run()
{
  if (_parseState == ParseScheme) {
    _parseScheme();
  }
  if (_parseState == ParseAuthority) {
    _parseAuthority();
  }
  if (_parseState == ParsePath) {
    _parsePath();
  }
  if (_parseState == ParseQuery) {
    _parseQuery();
  }
  if (_parseState == ParseFragment) {
    _parseFragment();
  }
  if (_parseState == ParseDone) {
    _client->getRequest().setUri(_tmpUri);
    getContext()->getStateHandler().setState<ParseVersion>();
    return;
  }
}

void ParseUri::_updateState(ParseUriState nextState)
{
  if (_sequence != NULL) {
    delete _sequence;
    _sequence = NULL;
  }
  _results.clear();
  _initParser = true;
  _parseState = nextState;
  _buffReader.resetPosInBuff();
}

/**
 * @brief Parse the scheme part of the uri
 */
void ParseUri::_parseScheme()
{
  if (_initParser) {
    _initParser = false;

    _sequence = schemePartRule();

    _sequence->setBufferReader(&_buffReader);
    _sequence->setResultMap(&_results);
    _buffReader.resetPosInBuff();
  }

  if (!_sequence->matches()) {
    _updateState(ParseAuthority);
    return;
  }
  if (_sequence->end()) {
    std::string scheme = _extractPartIfAvailable(SchemePart);
    scheme = scheme.substr(0, scheme.size() - 1); // remove "http:" -> "http"
    _tmpUri.setScheme(scheme);
    _updateState(ParseAuthority);
  }
}

/**
 * @brief Parse the authority part of the uri
 */
void ParseUri::_parseAuthority()
{
  if (_initParser) {
    _initParser = false;
    _sequence = authorityPartRule();
    _sequence->setBufferReader(&_buffReader);
  }

  if (!_sequence->matches()) {
    // TODO alarm
    _updateState(ParsePath);
    return;
  }
  if (_sequence->end()) {
    _tmpUri.setAuthority(_extractPartWithoutCurrChar());
    _updateState(ParsePath);
  }
}

void ParseUri::_parsePath()
{
  if (_initParser) {
    _initParser = false;
    _sequence = pathPartRule();
    _sequence->setBufferReader(&_buffReader);
  }

  if (!_sequence->matches()) {
    _updateState(ParseDone);
    return;
  }
  if (_sequence->end()) {
    std::string path = _extractPartWithoutCurrChar();
    ft::trim(path);
    _tmpUri.setPath(path);
    _updateState(ParseQuery);
  }
}

void ParseUri::_parseQuery()
{
  _updateState(ParseFragment);
}

void ParseUri::_parseFragment()
{
  _updateState(ParseDone);
}

std::string ParseUri::_extractPartIfAvailable(Rule::RuleId ruleId)
{
  try {
    const RuleResult& result = _results.at(ruleId);
    const long index = result.getEnd();
    if (index < 0) {
      return std::string("");
    }
    _buffReader.resetPosInBuff();
    return _client->getInBuff().consume(index + 1);
  } catch (const std::out_of_range& oor) {
    return std::string("");
  }
}

std::string ParseUri::_extractPart()
{
  const long index = _buffReader.getPosInBuff();
  if (index < 0) {
    return std::string("");
  }
  _buffReader.resetPosInBuff();
  return _client->getInBuff().consume(index + 1);
}

std::string ParseUri::_extractPartWithoutCurrChar()
{
  long index = _buffReader.getPosInBuff();
  index--;
  if (index < 0) {
    return std::string("");
  }
  return _client->getInBuff().consume(index + 1);
}
