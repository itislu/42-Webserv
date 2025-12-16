#include "CgiWriteEventHandler.hpp"

#include <client/Client.hpp>
#include <event/EventHandler.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <socket/SocketManager.hpp>
#include <utils/logger/Logger.hpp>

#include <exception>

/* ***************************************************************************/
// INIT

Logger& CgiWriteEventHandler::_log = Logger::getInstance(LOG_SERVER);

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
  if (!_client->alive() || _client->getCgiContext() == FT_NULLPTR) {
    return Disconnect;
  }

  Result result = Alive;
  if (isPollOutEvent(revents)) {
    result = _handlePollOutEvent();
    updateLastActivity();
  } else if (isPollHupEvent(revents) || isPollErrEvent(revents)) {
    result = Disconnect;
  }

  if (result == Disconnect) {
    _log.info() << "CgiWriteEventHandler: disconnect\n";
  }
  return result;
} catch (const std::exception& e) {
  _log.error() << "CgiWriteEventHandler exception: " << e.what() << '\n';
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  return Disconnect;
}

CgiWriteEventHandler::Result CgiWriteEventHandler::onTimeout()
{
  if (!_client->alive() || _client->getCgiContext() == FT_NULLPTR) {
    return Disconnect;
  }

  CgiContext& cgi = *_client->getCgiContext();
  cgi.setTimeoutWrite(true);
  return Disconnect;
}

long CgiWriteEventHandler::getTimeout() const
{
  return _client->getTimeout();
}

/* ************************************************************************** */
// PRIVATE

CgiWriteEventHandler::Result CgiWriteEventHandler::_handlePollOutEvent()
{
  // Write data
  CgiContext* cgiContext = _client->getCgiContext().get();
  if (cgiContext == FT_NULLPTR) {
    return Disconnect;
  }

  cgiContext->getShExecCgi().getState()->run();
  if (cgiContext->getShExecCgi().isDone()) {
    return Disconnect;
  }

  // all sent so far -> disable pollout
  if (_client->getRequest().getBody().isEmpty()) {
    SocketManager::getInstance().disablePollout(
      cgiContext->getPipeClientToCgi().getWriteFd());
  }
  return Alive;
}
