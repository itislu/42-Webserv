#include "ParseMethod.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/states/readRequestLine/ParseUri.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/ctype.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
// INIT

Logger& ParseMethod::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ParseMethod::ParseMethod(ReadRequestLine* context)
  : IState<ReadRequestLine>(context)
  , _client(context->getContext())
  , _buffReader()
{
  _log.info() << *_client << " ParseMethod\n";
  _init();
}

/**
 * @brief Parse method of first line
 *
 * 'METHOD SP URI SP VERSION CRLF'
 */
void ParseMethod::run()
{
  _sequence.reset();
  _buffReader.resetPosInBuff();
  if (!_sequence.matches()) {
    _log.error() << "ParseMethod: BadRequest\n";
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_methodNotImplemented()) {
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_sequence.reachedEnd()) {
    _extractMethod();
    getContext()->getStateHandler().setState<ParseUri>();
    return;
  }
}

/* ************************************************************************** */
// PRIVATE

void ParseMethod::_init()
{
  _buffReader.init(&_client->getInBuff());

  ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(ft::isupper));
  _sequence.addRule(ft::move(rep));
  _sequence.addRule(ft::make_shared<LiteralRule>(" "));

  _sequence.setBufferReader(&_buffReader);
}

/**
 * @brief Extract method from buffer and add it to the request struct
 */
void ParseMethod::_extractMethod()
{
  const std::size_t index = _buffReader.getPosInBuff();
  std::string methodStr = _client->getInBuff().consumeFront(index);
  const Request::Method method = Request::strToMethod(ft::trim(methodStr));
  _client->getRequest().setMethod(method);
}

bool ParseMethod::_methodNotImplemented()
{
  if (_buffReader.getPosInBuff() > Request::MaxMethodLen + 1) {
    _log.error() << "Method not implemented\n";
    _client->getResponse().setStatusCode(StatusCode::NotImplemented);
    return true;
  }
  return false;
}
