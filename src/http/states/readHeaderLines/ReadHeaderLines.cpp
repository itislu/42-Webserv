#include "ReadHeaderLines.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/readBody/ReadBody.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/memory.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/logger/LoggerHandler.hpp>
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
  , _log(&LoggerHandler::getInstance(log::http))
  , _done(false)
{
  _log->info("ReadHeaderLines");
  _init();
}

/* ************************************************************************** */
// PRIVATE

/**
 * @brief ### Read header lines of a HTTP request
 *
 * HTTP-message   = request-line CRLF
 *                  *( field-line CRLF )
 *                  CRLF
 *                  [ message-body ]
 */
void ReadHeaderLines::run()
{
  _readLines();
  if (_done) {
    _setNextState();
    return;
  }
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
        _done = true;
        return;
      }
    } else {
      _log->error("ReadHeaderLines: Bad Request: Headers:");
      _log->error(_client->getRequest().getHeaders().toString());
      _client->getResponse().setStatusCode(StatusCode::BadRequest);
      _done = true;
      return;
    }
  }
}

void ReadHeaderLines::_setNextState()
{
  if (_client->getResponse().getStatusCode() == StatusCode::Ok) {
    if (_client->getRequest().getMethod() == Request::POST) {
      _client->getStateHandler().setState<ReadBody>();
      return;
    }
  }
  _client->getStateHandler().setState<PrepareResponse>();
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
  const std::size_t index = line.find(':');
  if (index != std::string::npos) {
    const std::string name = line.substr(0, index);
    const std::string value = line.substr(index + 1, line.size());
    Headers& headers = _client->getRequest().getHeaders();
    headers.addHeader(name, value);
  }
}
