#include "ProcessCgiResponse.hpp"

#include <client/Client.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/Pipe.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
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
{
  _log.info() << *_client << " ProcessCgiResponse\n";
  _smartBuffer = ft::make_shared<SmartBuffer>();
}

void ProcessCgiResponse::run()
try {
  _log.info() << *_client << " ProcessCgiResponse run\n";
  _readCgiResponse();
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

void ProcessCgiResponse::_readCgiResponse()
{
  Pipe& pipe = getContext()->getPipeCgiToClient();

  // Read FROM CHILD (stdout)
  IInBuffer::RawBytes buffer(1024);
  const ssize_t bytesRead =
    read(pipe.getReadFd(), buffer.data(), buffer.size());

  _log.info() << "read " << bytesRead << "\n";
  if (bytesRead < 0) {
    _done = true;
    pipe.closeRead();
    Response& response = _client->getResponse();
    response.setBody(ft::move(_smartBuffer));
  } else if (bytesRead > 0) {
    _done = true;
    _smartBuffer->append(buffer, bytesRead);
    _log.info() << "Body: '" << buffer.data() << "'\n";
  }
}
