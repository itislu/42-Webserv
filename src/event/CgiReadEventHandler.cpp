#include "CgiReadEventHandler.hpp"
#include "socket/SocketManager.hpp"

#include <client/Client.hpp>
#include <event/EventHandler.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/StateHandler.hpp>

#include <exception>

/* ***************************************************************************/
// INIT

Logger& CgiReadEventHandler::_log = Logger::getInstance(LOG_SERVER);

/* ************************************************************************** */
// PUBLIC

CgiReadEventHandler::CgiReadEventHandler(int fdes,
                                         ft::shared_ptr<Client> client)
  : EventHandler(fdes)
  , _client(ft::move(client))
{
}

CgiReadEventHandler::Result CgiReadEventHandler::handleEvent(unsigned revents)
try {
  if (_client == FT_NULLPTR || !_client->alive() ||
      _client->getCgiContext() == FT_NULLPTR) {
    return Disconnect;
  }

  Result result = Alive;
  if (isPollInEvent(revents)) {
    result = _handlePollInEvent();
    updateLastActivity();
  } else if (isPollHupEvent(revents) || isPollErrEvent(revents)) {
    result = _handlePollHupOrErr();
  }

  if (result == Disconnect) {
    _log.info() << "CgiReadEventHandler: disconnect\n";
  }
  return result;
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

CgiReadEventHandler::Result CgiReadEventHandler::_handlePollInEvent()
{
  // Receive data
  CgiContext* cgiContext = _client->getCgiContext().get();
  if (cgiContext != FT_NULLPTR) {
    cgiContext->getShProcessCgiResponse().getState()->run();
    if (cgiContext->getShProcessCgiResponse().isDone()) {
      if (_client->alive()) {
        SocketManager::getInstance().enablePollout(_client->getFd());
      }
      return Disconnect;
    }
  } else {
    return Disconnect;
  }
  return Alive;
}

CgiReadEventHandler::Result CgiReadEventHandler::_handlePollHupOrErr()
{
  // Finish state
  CgiContext* cgiContext = _client->getCgiContext().get();
  if (cgiContext != FT_NULLPTR) {
    cgiContext->setCgiReadEventHandlerReceivedPollHupErr(true);
    cgiContext->getShProcessCgiResponse().getState()->run();
    if (_client->alive()) {
      SocketManager::getInstance().enablePollout(_client->getFd());
    }
  }
  return Disconnect;
}
