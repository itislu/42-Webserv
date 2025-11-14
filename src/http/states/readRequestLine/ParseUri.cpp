#include "ParseUri.hpp"

#include <client/Client.hpp>
#include <http/Authority.hpp>
#include <http/StatusCode.hpp>
#include <http/Uri.hpp>
#include <http/abnfRules/http11Rules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/readRequestLine/ParseVersion.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <utils/Buffer.hpp>
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

  if (_sequence().reachedEnd()) {
    _extractParts(results);
    _client->getRequest().setUri(_tmpUri);
    getContext()->getStateHandler().setState<ParseVersion>();
    return;
  }
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
  static Extractor<Uri> extractor;
  static bool init = false;
  if (!init) {
    init = true;
    extractor.addMapItem(AbsolutePath, &Uri::setPath);
    extractor.addMapItem(Query, &Uri::setQuery);
  }
  return extractor;
}

Extractor<Uri>& ParseUri::_uriExtractorAbsoluteForm()
{
  static Extractor<Uri> extractor;
  static bool init = false;
  if (!init) {
    init = true;
    extractor.addMapItem(Scheme, &Uri::setScheme);
    extractor.addMapItem(HierPartPathAbEmpty, &Uri::setPath);
    extractor.addMapItem(Query, &Uri::setQuery);
  }
  return extractor;
}

Extractor<Authority>& ParseUri::_authExtractor()
{
  static Extractor<Authority> extractor;
  static bool init = false;
  if (!init) {
    init = true;
    extractor.addMapItem(UserInfo, &Authority::setUserinfo);
    extractor.addMapItem(Host, &Authority::setHost);
    extractor.addMapItem(Port, &Authority::setPort);
  }
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
  const std::size_t indexInBuff = _buffReader.getPosInBuff();
  _client->getInBuff().remove(indexInBuff);
}
