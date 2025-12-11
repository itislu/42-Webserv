#include "CgiWriteEventHandler.hpp"

#include <client/Client.hpp>
#include <event/EventHandler.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/utility.hpp>
#include <utils/logger/Logger.hpp>

#include <exception>

/* ***************************************************************************/
// INIT

Logger& CgiWriteEventHandler::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

CgiWriteEventHandler::CgiWriteEventHandler(int fdes, Client* client)
  : EventHandler(fdes)
  , _client(client)
{
}

CgiWriteEventHandler::Result CgiWriteEventHandler::handleEvent(unsigned revents)
try {
  updateLastActivity();

  if (!isPollOutEvent(revents)) {
    // should only be used for write
    return Disconnect;
  }

  // Write data
  CgiContext* cgiContext = _client->getCgiContext().get();
  if (cgiContext != FT_NULLPTR) {
    cgiContext->getShExecCgi().getState()->run();
    if (cgiContext->getShExecCgi().isDone()) {
      return Disconnect;
    }
  } else {
    return Disconnect;
  }
  return Alive;
} catch (const std::exception& e) {
  _log.error() << "CgiWriteEventHandler exception: " << e.what() << '\n';
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  return Disconnect;
}

long CgiWriteEventHandler::getTimeout() const
{
  return _client->getTimeout();
}

/* ************************************************************************** */
// PRIVATE
