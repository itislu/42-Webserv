#include "ReadHeaderLines.hpp"
#include "RequestHeaderValidator.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/validateRequest/ValidateRequest.hpp>
#include <http/utils/HeaderParser.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInOutBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>

/* ************************************************************************** */
// INIT

Logger& ReadHeaderLines::_log = Logger::getInstance(LOG_HTTP);

const std::size_t ReadHeaderLines::_defaultMaxFieldLineSize;
const std::size_t ReadHeaderLines::_defaultMaxHeaderSize;
std::size_t ReadHeaderLines::_maxFieldLineSize = _defaultMaxFieldLineSize;
std::size_t ReadHeaderLines::_maxHeaderSize = _defaultMaxHeaderSize;

/* ************************************************************************** */
// PUBLIC

ReadHeaderLines::ReadHeaderLines(Client* context)
  : IState(context)
  , _client(context)
  , _done(false)
{
  _log.info() << *_client << " ReadHeaderLines\n";
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
try {
  _readLines();

  const StatusCode& statusCode = _client->getResponse().getStatusCode();
  if (!statusCode.isSuccessCode()) {
    getContext()->getStateHandler().setState<PrepareResponse>();
  } else if (_done) {
    getContext()->getStateHandler().setState<ValidateRequest>();
  }
} catch (const IBuffer::BufferException& e) {
  _log.error() << *_client << " ReadHeaderLines: " << e.what() << "\n";
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setState<PrepareResponse>();
}

void ReadHeaderLines::setMaxFieldLineSize(std::size_t value)
{
  _maxFieldLineSize = value;
}

void ReadHeaderLines::setMaxHeaderSize(std::size_t value)
{
  _maxHeaderSize = value;
}

void ReadHeaderLines::resetMaxFieldLineSize()
{
  _maxFieldLineSize = _defaultMaxFieldLineSize;
}

void ReadHeaderLines::resetMaxHeaderSize()
{
  _maxHeaderSize = _defaultMaxHeaderSize;
}

void ReadHeaderLines::_init()
{
  ft::shared_ptr<RequestHeaderValidator> validator =
    ft::make_shared<RequestHeaderValidator>(_client);
  validator->setMaxFieldLineSize(_maxFieldLineSize);
  validator->setMaxHeaderSize(_maxHeaderSize);
  _headerParser.setValidator(ft::move(validator));
}

void ReadHeaderLines::_readLines()
{
  Response& response = _client->getResponse();
  IInOutBuffer& buffer = _client->getInBuff();
  Headers& headers = _client->getRequest().getHeaders();
  const HeaderParser::Result result =
    _headerParser.parseIntoStruct(buffer, headers);

  switch (result) {
    case HeaderParser::EndOfBuffer:
      // header is not complete
      break;
    case HeaderParser::EndOfHeaders:
      _done = true;
      break;
    case HeaderParser::SyntaxError:
      _log.error() << "ReadHeaderLines: syntax error\n";
      response.setStatusCode(StatusCode::BadRequest);
      break;
    case HeaderParser::FieldLineTooLarge:
      response.setStatusCode(StatusCode::RequestHeaderFieldsTooLarge);
      _log.error() << "ReadHeaderLines: field line too large\n";
      break;
    case HeaderParser::HeaderTooLarge:
      response.setStatusCode(StatusCode::RequestHeaderFieldsTooLarge);
      _log.error() << "ReadHeaderLines: header too large\n";
      break;
    case HeaderParser::InvalidHeader:
      // Error code will be set by RequestHeaderValidator
      if (response.getStatusCode().isSuccessCode()) {
        _log.error()
          << "ReadHeaderLines: validator failed to set error status\n";
        response.setStatusCode(StatusCode::BadRequest);
      }
      break;
  }
}
