#include "ParseMethod.hpp"
#include "utils/abnfRules/LiteralRule.hpp"
#include "utils/abnfRules/RangeRule.hpp"
#include "utils/abnfRules/RepetitionRule.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/states/readStartLine/ParseUri.hpp>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <libftpp/string.hpp>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
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
  , _init(true)
{
  _buffReader.init(&_client->getInBuff());
}

ParseMethod::~ParseMethod() {}

/**
 * @brief Parse method of first line
 *
 * 'METHOD SP URI SP VERSION CRLF'
 */
void ParseMethod::run()
{
  if (_init) {
    _init = false;
    RepetitionRule* rep = new RepetitionRule(new RangeRule(::isupper));
    rep->setMax(static_cast<int>(Request::MAX_METHOD_LEN));
    _sequenze.addRule(rep);
    _sequenze.addRule(new LiteralRule(" "));
    _sequenze.setBufferReader(&_buffReader);
  }

  if (!_sequenze.matches()) {
    // TODO ALARM
    getContext()->getStateHandler().setDone();
    return;
  }

  if (_sequenze.end()) {
    _extractMethod();
    getContext()->getStateHandler().setState<ParseUri>();
    return;
  }
}

/* ************************************************************************** */
// PRIVATE

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
