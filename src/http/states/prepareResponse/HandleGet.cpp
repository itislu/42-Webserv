#include "HandleGet.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/headerUtils.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/memory.hpp>
#include <utils/buffer/StaticFileBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <string>

/* ************************************************************************** */
// INIT

Logger& HandleGet::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandleGet::HandleGet(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandleGet\n";
  _log.info() << "HandleGet: " << _client->getResource().getPath() << "\n";
}

void HandleGet::run()
{
  if (_client->getResource().getType() == Resource::Autoindex) {
    _handleAutoIndex();
  } else {
    _handleStaticFile();
  }
  _setNextState();
}

/* ************************************************************************** */
// PRIVATE

void HandleGet::_setNextState()
{
  const StatusCode& statusCode = _client->getResponse().getStatusCode();
  if (statusCode == StatusCode::Ok) {
    getContext()->getStateHandler().setDone();
  } else {
    getContext()->getStateHandler().setState<HandleError>();
  }
}

void HandleGet::_handleAutoIndex()
{
  // todo
}

void HandleGet::_handleStaticFile()
{
  Response& response = _client->getResponse();

  // set body
  const std::string& filePath = _client->getResource().getPath();
  response.setBody(ft::make_shared<StaticFileBuffer>(filePath));

  // set headers
  setContentLengthHeader(response.getHeaders(), response.getBody()->size());
  setContentTypeHeader(response.getHeaders(), filePath);
}
