#include "HandleRedirect.hpp"
#include "http/Headers.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

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
