#include "CgiReadEventHandler.hpp"

#include <client/Client.hpp>
#include <event/EventHandler.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <socket/SocketManager.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/StateHandler.hpp>

#include <algorithm>
#include <exception>
#include <string>

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
  if (!_client->alive() || _client->getCgiContext() == FT_NULLPTR) {
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
    _log.info() << logName() << "disconnect\n";
  }
  return result;
} catch (const std::exception& e) {
  _log.error() << logName() << "exception: " << e.what() << '\n';
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  return Disconnect;
}

CgiReadEventHandler::Result CgiReadEventHandler::onTimeout()
{
  _log.info() << logName() << "onTimeout\n";
  if (!_client->alive() || _client->getCgiContext() == FT_NULLPTR) {
    return Disconnect;
  }

  CgiContext& cgi = *_client->getCgiContext();
  cgi.setTimeoutRead(true);
  return Disconnect;
}

long CgiReadEventHandler::getTimeout() const
{
  const long oneSecond = 1;
  return std::max(_client->getTimeout() - oneSecond, oneSecond);
}

std::string CgiReadEventHandler::logName() const
{
  std::string name = "CgiReadEventHandler(";
  name.append(ft::to_string(getFd()));
  name.append("): ");
  return name;
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
