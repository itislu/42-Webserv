#include "ParseUri.hpp"

#include <client/Client.hpp>
#include <http/StatusCode.hpp>
#include <http/Uri.hpp>
#include <http/abnfRules/requestLineRules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/states/readRequestLine/ParseVersion.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <string>

/* ************************************************************************** */
// INIT

Logger& ParseUri::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ParseUri::ParseUri(ReadRequestLine* context)
  : IState<ReadRequestLine>(context)
  , _client(context->getContext())
  , _parseState(ParseScheme)
  , _buffReader()
  , _initParser(true)
{
  _log.info() << "ParseUri\n";
  _buffReader.init(&_client->getInBuff());
}

/* ************************************************************************** */
// PRIVATE

void ParseUri::run()
{
  if (_sequence != FT_NULLPTR) {
    _sequence->reset();
  }
  _buffReader.resetPosInBuff();
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
  _sequence = FT_NULLPTR;
  _initParser = true;
  _parseState = nextState;
}

void ParseUri::_initSequence()
{
  // Buffer
  _buffReader.resetPosInBuff();
  _sequence->setBufferReader(&_buffReader);

  // Results
  _results.clear();
  _sequence->setResultMap(&_results);
}

/**
 * @brief Parse the scheme part of the uri
 */
void ParseUri::_parseScheme()
{
  if (_initParser) {
    _initParser = false;
    _sequence = schemePartRule();
    _initSequence();
  }

  if (!_sequence->matches()) {
    _log.info() << "No scheme found\n";
    _updateState(ParseAuthority);
    return;
  }
  if (_sequence->reachedEnd()) {
    if (_partFound(SchemePart)) {
      std::string scheme = _extractPart(SchemePart);
      scheme = scheme.substr(0, scheme.size() - 1); // remove "http:" -> "http"
      _tmpUri.setScheme(scheme);
    }
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
    _initSequence();
  }

  if (!_sequence->matches()) {
    _log.info() << "No authority found\n";
    _updateState(ParsePath);
    return;
  }
  if (_sequence->reachedEnd()) {
    if (_partFound(AuthorityPart)) {
      std::string auth = _extractPartWithoutCurrChar(AuthorityPart);
      auth = auth.substr(2, auth.size()); // remove '//'
      _tmpUri.setAuthority(auth);
    }
    _updateState(ParsePath);
  }
}

void ParseUri::_parsePath()
{
  if (_initParser) {
    _initParser = false;
    _sequence = pathPartRule();
    _initSequence();
  }

  if (!_sequence->matches()) {
    _log.info() << "Bad Request invalid path\n";
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    _updateState(ParseDone);
    return;
  }
  if (_sequence->reachedEnd()) {
    if (_partFound(PathPart)) {
      std::string path = _extractPartWithoutCurrChar(PathPart);
      ft::trim(path);
      _tmpUri.setPath(path);
    }
    _updateState(ParseQuery);
  }
}

void ParseUri::_parseQuery()
{
  if (_initParser) {
    _initParser = false;
    _sequence = queryPartRule();
    _initSequence();
  }

  if (!_sequence->matches()) {
    _log.info() << "No query found\n";
    _updateState(ParseFragment);
    return;
  }
  if (_sequence->reachedEnd()) {
    if (_partFound(QueryPart)) {
      std::string query = _extractPartWithoutCurrChar(QueryPart);
      ft::trim(query);
      _tmpUri.setQuery(query);
    }
    _updateState(ParseFragment);
  }
}

void ParseUri::_parseFragment()
{
  if (_initParser) {
    _initParser = false;
    _sequence = fragmentPartRule();
    _initSequence();
  }

  if (!_sequence->matches()) {
    _log.info() << "No fragment found\n";
    _updateState(ParseDone);
    return;
  }

  if (_sequence->reachedEnd()) {
    if (_partFound(FragmentPart)) {
      std::string frag = _extractPartWithoutCurrChar(FragmentPart);
      ft::trim(frag);
      _tmpUri.setFragment(frag);
    }
    _updateState(ParseDone);
  }
}

std::string ParseUri::_extractPart(const Rule::RuleId& ruleId)
{
  const RuleResult& result = _results[ruleId];
  const long index = result.getEnd();
  return _client->getInBuff().consume(index + 1);
}

std::string ParseUri::_extractPartWithoutCurrChar(const Rule::RuleId& ruleId)
{
  const RuleResult& result = _results[ruleId];
  const long index = result.getEnd() - 1;
  return _client->getInBuff().consume(index + 1);
}

bool ParseUri::_partFound(const Rule::RuleId& ruleId)
{
  return _results.find(ruleId) != _results.end();
}
