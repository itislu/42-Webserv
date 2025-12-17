#include "HandleRedirect.hpp"
#include "http/Headers.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/headerUtils.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/optional.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
#include <utils/buffer/StaticFileBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <exception>
#include <sstream>
#include <string>

/* ************************************************************************** */
// INIT

Logger& HandleRedirect::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandleRedirect::HandleRedirect(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandleRedirect\n";
}

void HandleRedirect::run()
{
  Response& response = _client->getResponse();

  const std::string& path = _client->getResource().getPath();
  _log.info() << "PATH: " << path << "\n";

  response.getHeaders().setHeader("Location", path);
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
