#include "CgiReadEventHandler.hpp"

#include <client/Client.hpp>
#include <event/EventHandler.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/utility.hpp>
#include <utils/logger/Logger.hpp>

#include <exception>

/* ***************************************************************************/
// INIT

Logger& CgiReadEventHandler::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

CgiReadEventHandler::CgiReadEventHandler(int fdes, Client* client)
  : EventHandler(fdes)
  , _client(client)
{
}

CgiReadEventHandler::Result CgiReadEventHandler::handleEvent(unsigned revents)
try {
  updateLastActivity();

  if (!isPollInEvent(revents)) {
    // should only be used for read
    return Disconnect;
  }

  // Receive data
  CgiContext* cgiContext = _client->getCgiContext().get();
  if (cgiContext != FT_NULLPTR) {
    cgiContext->getShProcessCgiResponse().getState()->run();
    if (cgiContext->getShProcessCgiResponse().isDone()) {
      return Disconnect;
    }
  } else {
    return Disconnect;
  }
  return Alive;
} catch (const std::exception& e) {
  _log.error() << "CgiReadEventHandler exception: " << e.what() << '\n';
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  return Disconnect;
}

long CgiReadEventHandler::getTimeout() const
{
  return _client->getTimeout();
}

/* ************************************************************************** */
// PRIVATE
