#include "ParseMethod.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/states/readRequestLine/ParseUri.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

#include <ctype.h>
#include <string>

/* ************************************************************************** */
// INIT

Logger& ParseMethod::_log = Logger::getInstance(LOG_HTTP);
ft::shared_ptr<SequenceRule> ParseMethod::_sequence = FT_NULLPTR;

/* ************************************************************************** */
// PUBLIC

ParseMethod::ParseMethod(ReadRequestLine* context)
  : IState<ReadRequestLine>(context)
  , _client(context->getContext())
  , _buffReader()
{
  _log.info() << "ParseMethod\n";
  _initSequence();
  _initState();
}

/**
 * @brief Parse method of first line
 *
 * 'METHOD SP URI SP VERSION CRLF'
 */
void ParseMethod::run()
{
  if (_sequence == FT_NULLPTR) {
    getContext()->getStateHandler().setDone();
    return; // TODO Alarm
  }

  _sequence->reset();
  _buffReader.resetPosInBuff();
  if (!_sequence->matches()) {
    _log.error() << "Bad Request\n";
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_sequence->reachedEnd()) {
    _extractMethod();
    if (_client->getResponse().getStatusCode() == StatusCode::Ok) {
      getContext()->getStateHandler().setState<ParseUri>();
    } else {
      getContext()->getStateHandler().setDone();
    }
    return;
  }
}

/* ************************************************************************** */
// PRIVATE

/**
 * METHOD SP
 */
void ParseMethod::_initSequence()
{
  if (_sequence == FT_NULLPTR) {
    _sequence = ft::make_shared<SequenceRule>();

    ft::shared_ptr<RepetitionRule> rep =
      ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(::isupper));

    _sequence->addRule(ft::move(rep));
    _sequence->addRule(ft::make_shared<LiteralRule>(" "));
  }
}

void ParseMethod::_initState()
{
  _buffReader.init(&_client->getInBuff());
  _sequence->setBufferReader(&_buffReader);
}

/**
 * @brief Extract method from buffer and add it to the request struct
 */
void ParseMethod::_extractMethod()
{
  const long index = _buffReader.getPosInBuff();
  std::string strMethod = _client->getInBuff().consume(index + 1);

  if (strMethod.size() > Request::MaxMethodLen) {
    _client->getResponse().setStatusCode(StatusCode::NotImplemented);
    return;
  }

  ft::trim(strMethod);
  const Request::Method method = Request::strToMethod(strMethod);
  _client->getRequest().setMethod(method);
}
