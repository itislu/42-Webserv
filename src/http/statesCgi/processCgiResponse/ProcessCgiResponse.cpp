#include "ProcessCgiResponse.hpp"

#include <client/Client.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/headerUtils.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/statesCgi/processCgiResponse/CgiHeaderValidator.hpp>
#include <http/utils/HeaderParser.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/Pipe.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <exception>
#include <sys/types.h>
#include <unistd.h>

/* ************************************************************************** */
// INIT

Logger& ProcessCgiResponse::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ProcessCgiResponse::ProcessCgiResponse(CgiContext* context)
  : IState<CgiContext>(context)
  , _client(context->getClient())
  , _done(false)
  , _headersParsed(false)
{
  _log.info() << *_client << " ProcessCgiResponse\n";
  _smartBuffer = ft::make_shared<SmartBuffer>();

  _headerParser.setValidator(ft::make_shared<CgiHeaderValidator>(_client));
}

void ProcessCgiResponse::run()
try {
  if (getContext()->cgiReadEventHandlerReceivedPollHupErr()) {
    _done = true;
  }

  if (!_done) {
    _readCgiResponse();
  }

  if (!_done && _ok()) {
    // when headers are parsed, _smartbuffer is connected to response body
    if (!_headersParsed) {
      _parseCgiHeader();
    }
  }
  if (_done && _ok()) {
    _checkBody();
  }

  if (!_ok()) {
    _log.info() << *_client << " ProcessCgiResponse failed\n";
    getContext()->getShProcessCgiResponse().setDone();
  }
  if (_done) {
    _log.info() << *_client << " ProcessCgiResponse done\n";
    getContext()->getShProcessCgiResponse().setDone();
  }
} catch (const std::exception& e) {
  Client* client = _client;
  _log.error() << *client << " ProcessCgiResponse: " << e.what() << "\n";
  client->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getShProcessCgiResponse().setDone();
}

bool ProcessCgiResponse::_ok()
{
  return _client->getResponse().getStatusCode() == StatusCode::Ok;
}

void ProcessCgiResponse::_readCgiResponse()
{
  Response& response = _client->getResponse();
  Pipe& pipe = getContext()->getPipeCgiToClient();

  // Read FROM CHILD (stdout)
  IInBuffer::RawBytes buffer(Client::maxChunk);
  const ssize_t bytesRead =
    read(pipe.getReadFd(), buffer.data(), buffer.size());
  if (bytesRead > 0) {
    _smartBuffer->append(buffer, bytesRead);
    _log.info() << "ProcessCgiResponse: read " << bytesRead << " bytes\n";
  } else if (bytesRead == 0) {
    _done = true;
    _log.info() << "ProcessCgiResponse: read returned 0 (EOF)\n";
  } else {
    pipe.closeRead();
    response.setStatusCode(StatusCode::InternalServerError);
    _log.error() << "ProcessCgiResponse: read from child failed\n";
  }
}

void ProcessCgiResponse::_parseCgiHeader()
{
  Response& response = _client->getResponse();
  const HeaderParser::Result result =
    _headerParser.parseIntoStruct(*_smartBuffer, response.getHeaders());

  switch (result) {
    case HeaderParser::EndOfBuffer:
      // header is not complete
      break;
    case HeaderParser::EndOfHeaders:
      _headersParsed = true;
      _client->getResponse().setBody(_smartBuffer);
      break;
    case HeaderParser::SyntaxError:
      _log.error() << "ProcessCgiResponse: header syntax error\n";
      response.setStatusCode(StatusCode::BadGateway);
      break;
    case HeaderParser::FieldLineTooLarge:
      response.setStatusCode(StatusCode::BadGateway);
      _log.error() << "ProcessCgiResponse: header line too large\n";
      break;
    case HeaderParser::HeaderTooLarge:
      response.setStatusCode(StatusCode::BadGateway);
      _log.error() << "ProcessCgiResponse: header too large\n";
      break;
    case HeaderParser::InvalidHeader:
      if (response.getStatusCode() == StatusCode::Ok) {
        _log.error()
          << "ProcessCgiResponse: validator failed to set error status\n";
        response.setStatusCode(StatusCode::BadGateway);
      }
      break;
  }
}

void ProcessCgiResponse::_checkBody()
{
  // todo just set body size here?
  Response& response = _client->getResponse();
  if (response.getBody() == FT_NULLPTR || response.getBody()->isEmpty()) {
    ft::shared_ptr<IInBuffer> nullBuffer;
    response.setBody(ft::move(nullBuffer));
    setContentLengthHeader(response.getHeaders(), 0);
    return;
  }
  setContentLengthHeader(response.getHeaders(), response.getBody()->size());
}
