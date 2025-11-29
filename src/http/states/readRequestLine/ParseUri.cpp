#include "ParseUri.hpp"

#include <client/Client.hpp>
#include <http/Authority.hpp>
#include <http/StatusCode.hpp>
#include <http/Uri.hpp>
#include <http/abnfRules/http11Rules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/states/readRequestLine/ParseVersion.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Extractor.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>

/* ************************************************************************** */
// INIT

Logger& ParseUri::_log = Logger::getInstance(LOG_HTTP);
const std::size_t ParseUri::_defaultMaxUriLength;
std::size_t ParseUri::_maxUriLength = _defaultMaxUriLength;

/* ************************************************************************** */
// PUBLIC

ParseUri::ParseUri(ReadRequestLine* context)
  : IState<ReadRequestLine>(context)
  , _client(context->getContext())
  , _buffReader()
{
  _log.info() << "ParseUri\n";
  _buffReader.init(&_client->getInBuff());
}

void ParseUri::run()
{
  Rule::ResultMap results;

  _sequence().reset();
  _sequence().setBufferReader(&_buffReader);
  _sequence().setResultMap(&results);
  _buffReader.resetPosInBuff();

  if (!_sequence().matches()) {
    _log.error() << "Invalid request target\n";
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_uriTooLong()) {
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_sequence().reachedEnd()) {
    _extractParts(results);
    _client->getRequest().setUri(_tmpUri);
    getContext()->getStateHandler().setState<ParseVersion>();
    return;
  }
}

void ParseUri::setMaxUriLength(std::size_t value)
{
  _maxUriLength = value;
}

void ParseUri::resetMaxUriLength()
{
  _maxUriLength = _defaultMaxUriLength;
}

/* ************************************************************************** */
// PRIVATE

/**
 * sequence is: request-target SP
 */
SequenceRule& ParseUri::_sequence()
{
  static SequenceRule sequence;
  static bool init = false;
  if (!init) {
    init = true;
    sequence.addRule(requestTargetRule());
    sequence.addRule(ft::make_shared<LiteralRule>(" "));
  }
  return sequence;
}

bool ParseUri::_isAbsoluteForm(const Rule::ResultMap& results)
{
  const Rule::ResultMap::const_iterator iter = results.find(AbsoluteForm);
  return iter != results.end();
}

Extractor<Uri>& ParseUri::_uriExtractorOriginForm()
{
  static Extractor<Uri> extractor = Extractor<Uri>()
                                      .addMapItem(AbsolutePath, &Uri::setPath)
                                      .addMapItem(Query, &Uri::setQuery);
  return extractor;
}

Extractor<Uri>& ParseUri::_uriExtractorAbsoluteForm()
{
  static Extractor<Uri> extractor =
    Extractor<Uri>()
      .addMapItem(Scheme, &Uri::setScheme)
      .addMapItem(HierPartPathAbEmpty, &Uri::setPath)
      .addMapItem(Query, &Uri::setQuery);
  return extractor;
}

Extractor<Authority>& ParseUri::_authExtractor()
{
  static Extractor<Authority> extractor =
    Extractor<Authority>()
      .addMapItem(UserInfo, &Authority::setUserinfo)
      .addMapItem(Host, &Authority::setHost)
      .addMapItem(Port, &Authority::setPort);
  return extractor;
}

void ParseUri::_extractParts(const Rule::ResultMap& results)
{
  if (_isAbsoluteForm(results)) {
    _uriExtractorAbsoluteForm().run(_tmpUri, results, _client->getInBuff());
  } else {
    _uriExtractorOriginForm().run(_tmpUri, results, _client->getInBuff());
  }
  _authExtractor().run(_tmpUri.getAuthority(), results, _client->getInBuff());

  // remove bytes from buffer
  const std::size_t posInBuff = _buffReader.getPosInBuff();
  _client->getInBuff().removeFront(posInBuff);
}

bool ParseUri::_uriTooLong()
{
  if (_buffReader.getPosInBuff() > _maxUriLength) {
    _log.error() << "ParseUri: uri too long\n";
    _client->getResponse().setStatusCode(StatusCode::UriTooLong);
    return true;
  }
  return false;
}
