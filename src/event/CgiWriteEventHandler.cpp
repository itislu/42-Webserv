#include "CgiWriteEventHandler.hpp"

#include <client/Client.hpp>
#include <event/EventHandler.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/logger/Logger.hpp>

#include <exception>

/* ***************************************************************************/
// INIT

Logger& CgiWriteEventHandler::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

CgiWriteEventHandler::CgiWriteEventHandler(int fdes,
                                           ft::shared_ptr<Client> client)
  : EventHandler(fdes)
  , _client(ft::move(client))
{
}

CgiWriteEventHandler::Result CgiWriteEventHandler::handleEvent(unsigned revents)
try {
  if (_client == FT_NULLPTR || !_client->alive() ||
      _client->getCgiContext() == FT_NULLPTR) {
    return Disconnect;
  }

  if (!isPollOutEvent(revents)) {
    // should only be used for write
    _log.error() << "CgiWriteEventHandler: invalid event\n";
    return Disconnect;
  }

  _log.info() << "CgiWriteEventHandler: called\n";

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
  updateLastActivity();
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
