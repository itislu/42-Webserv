#include "ReadHeaderLines.hpp"
#include "http/StatusCode.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
// PUBLIC

ReadHeaderLines::ReadHeaderLines(Client* context)
  : IState(context)
  , _client(context)
  , _endOfLine()
  , _buffReader()
{
  _init();
}

/* ************************************************************************** */
// PRIVATE

/**
 * @brief ### Read header lines of a HTTP request
 *
 * HTTP-message   = start-line CRLF
 *                  *( field-line CRLF )
 *                  CRLF
 *                  [ message-body ]
 */
void ReadHeaderLines::run()
{
  _readLines();
}

void ReadHeaderLines::_init()
{
  _buffReader.init(&_client->getInBuff());

  _fieldLine = fieldLinePartRule();
  _fieldLine->setBufferReader(&_buffReader);
  _fieldLine->setResultMap(&_results);

  _endOfLine = endOfLineRule();
  _endOfLine->setBufferReader(&_buffReader);
  _endOfLine->setResultMap(&_results);
}

void ReadHeaderLines::_readLines()
{
  _buffReader.resetPosInBuff();
  while (!_buffReader.reachedEnd()) {
    _results.clear();
    _fieldLine->reset();
    _endOfLine->reset();

    if (_fieldLine->matches()) {
      if (_fieldLine->end()) {
        const std::string fieldLine = _extractPart(FieldLinePart);
        _addLineToHeaders(fieldLine);
      }
    } else if (_endOfLine->matches()) {
      if (_endOfLine->end()) {
        _extractPart(EndOfLine);
        getContext()->getStateHandler().setDone();
        return;
      }
    } else {
      _client->getResponse().setStatusCode(StatusCode::BadRequest);
      getContext()->getStateHandler().setDone();
      return;
    }
  }
}

std::string ReadHeaderLines::_extractPart(const Rule::RuleId& ruleId)
{
  const RuleResult& result = _results[ruleId];
  const long index = result.getEnd();
  _buffReader.resetPosInBuff();
  return _client->getInBuff().consume(index + 1);
}

void ReadHeaderLines::_addLineToHeaders(const std::string& line)
{
  // todo if header exit append value with ', '
  const std::size_t index = line.find(':');
  if (index != std::string::npos) {
    std::string name = line.substr(0, index);
    std::string value = line.substr(index + 1, line.size());
    ft::trim(name);
    ft::trim(value);
    Request::HeaderMap& headers = _client->getRequest().getHeaders();
    headers[name] = value;
  }
}
