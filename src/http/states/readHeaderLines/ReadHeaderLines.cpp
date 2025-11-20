#include "ReadHeaderLines.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/states/readBody/ReadBody.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/IBuffer.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/RuleResult.hpp>
#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
// INIT

Logger& ReadHeaderLines::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ReadHeaderLines::ReadHeaderLines(Client* context)
  : IState(context)
  , _client(context)
  , _buffReader()
  , _done(false)
{
  _log.info() << "ReadHeaderLines\n";
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

bool ReadHeaderLines::_readingOk()
{
  const StatusCode& statuscode = _client->getResponse().getStatusCode();
  if (statuscode != StatusCode::Ok) {
    return false;
  }
  if (_buffReader.fail()) {
    _client->getResponse().setStatusCode(StatusCode::InternalServerError);
    _log.error() << "ReadHeaderLines: _buffReader failed: "
                 << _buffReader.error().what() << "\n";
    return false;
  }
  if (_buffReader.reachedEnd()) {
    return false;
  }
  return true;
}

void ReadHeaderLines::_readLines()
{
  _buffReader.resetPosInBuff();
  while (_readingOk()) {
    _results.clear();
    _fieldLine->reset();
    _endOfLine->reset();

    if (_fieldLine->matches()) {
      if (_fieldLine->reachedEnd()) {
        const std::string fieldLine = _extractPart(FieldLinePart);
        _addLineToHeaders(fieldLine);
      }
    } else if (_hasEndOfLine()) {
      if (_endOfLine->reachedEnd()) {
        _extractPart(EndOfLine);
        _done = true;
        return;
      }
    } else {
      _log.error() << "ReadHeaderLines: Bad Request: Headers:\n";
      _log.error() << _client->getRequest().getHeaders().toString() << '\n';
      _client->getResponse().setStatusCode(StatusCode::BadRequest);
      _done = true;
      return;
    }
  }
}

void ReadHeaderLines::_setNextState()
{
  _client->getStateHandler().setState<ReadBody>();
}

bool ReadHeaderLines::_hasEndOfLine()
{
  _buffReader.resetPosInBuff();
  return _endOfLine->matches();
}

std::string ReadHeaderLines::_extractPart(const Rule::RuleId& ruleId)
{
  const RuleResult& result = _results[ruleId];
  const long index = result.getEnd();
  const IBuffer::ExpectStr res = _client->getInBuff().consumeFront(index);
  if (!res.has_value()) {
    _client->getResponse().setStatusCode(StatusCode::InternalServerError);
    _log.error() << "ReadHeaderLines: _extractPart: InternalServerError\n";
    return "";
  }
  _buffReader.resetPosInBuff();
  return *res;
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
