#include "ParseMethod.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/states/readRequestLine/ParseUri.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
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

#include <ctype.h>
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
  _log.info() << "ParseMethod\n";
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
    _log.error() << "Bad Request\n";
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
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
    ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(::isupper));
  rep->setMax(static_cast<int>(Request::MaxMethodLen));
  _sequence.addRule(ft::move(rep));
  _sequence.addRule(ft::make_shared<LiteralRule>(" "));

  _sequence.setBufferReader(&_buffReader);
}

/**
 * @brief Extract method from buffer and add it to the request struct
 */
void ParseMethod::_extractMethod()
{
  const long index = _buffReader.getPosInBuff();
  std::string input = _client->getInBuff().consumeFront(index);
  ft::trim(input);
  const Request::Method method = Request::strToMethod(input);
  _client->getRequest().setMethod(method);
}
