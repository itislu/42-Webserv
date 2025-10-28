#include "ParseMethod.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/states/readStartLine/ParseUri.hpp>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <libftpp/string.hpp>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

#include <cctype>
#include <string>

/* ************************************************************************** */
// PUBLIC

ParseMethod::ParseMethod(ReadStartLine* context)
  : IState<ReadStartLine>(context)
  , _client(context->getContext())
  , _buffReader()
{
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
    _client->getResponse().setStatusCode(StatusCode::BadRequest);
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_sequence.end()) {
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

  RepetitionRule* rep = new RepetitionRule(new RangeRule(::isupper));
  rep->setMax(static_cast<int>(Request::MaxMethodLen));
  _sequence.addRule(rep);
  _sequence.addRule(new LiteralRule(" "));

  _sequence.setBufferReader(&_buffReader);
}

/**
 * @brief Extract method from buffer and add it to the request struct
 */
void ParseMethod::_extractMethod()
{
  const long index = _buffReader.getPosInBuff();
  std::string strMethod = _client->getInBuff().consume(index + 1);
  ft::trim(strMethod);
  const Request::Method method = Request::strToMethod(strMethod);
  _client->getRequest().setMethod(method);
}
