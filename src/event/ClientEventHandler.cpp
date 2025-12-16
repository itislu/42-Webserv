#include "ClientEventHandler.hpp"
#include "http/Response.hpp"
#include "utils/process/ChildProcessManager.hpp"

#include <client/Client.hpp>
#include <event/CgiReadEventHandler.hpp>
#include <event/CgiWriteEventHandler.hpp>
#include <event/EventHandler.hpp>
#include <event/EventManager.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <socket/SocketManager.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/StateHandler.hpp>

#include <cstring>
#include <exception>
#include <sys/socket.h>

/* ***************************************************************************/
// INIT

Logger& ClientEventHandler::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ClientEventHandler::ClientEventHandler(int fdes, ft::shared_ptr<Client> client)
  : EventHandler(fdes)
  , _client(ft::move(client))
  , _cgiEventHandlerAdded(false)
  , _sending(false)
{
}

ClientEventHandler::Result ClientEventHandler::handleEvent(unsigned revents)
try {
  Result result = Alive;
  if (!_client->alive()) {
    result = Disconnect;
  }

  if (result == Alive) {
    if (isPollInEvent(revents)) {
      result = _handlePollInEvent();
    } else if (isPollOutEvent(revents)) {
      result = _handlePollOutEvent();
    } else if (isPollHupEvent(revents) || isPollErrEvent(revents)) {
      result = Disconnect;
    }
  }

  if (result != Alive) {
    _client->setAlive(false);
    _log.info() << *_client << " will disconnect\n";
  }
  return result;
} catch (const std::exception& e) {
  _log.error() << "ClientEventHandler exception: " << e.what() << '\n';
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  _client->setAlive(false);
  _handleException();
  return Disconnect;
}

ClientEventHandler::Result ClientEventHandler::onTimeout()
{
  _log.info() << "ClientEventHandler: onTimeout\n";
  if (_client->getCgiContext() == FT_NULLPTR) {
    return Disconnect;
  }

  CgiContext& cgi = *_client->getCgiContext();
  const bool writeDone = cgi.getShExecCgi().isDone();
  const bool readDone = cgi.getShProcessCgiResponse().isDone();
  if ((writeDone || cgi.timeoutWrite()) && (readDone || cgi.timeoutRead())) {
    _log.info() << "ClientEventHandler: onTimeout kill child\n";
    ChildProcessManager::getInstance().killChild(cgi.getChildPid());
    return Disconnect;
  }
  updateLastActivity();
  return TimeoutExtended;
}

long ClientEventHandler::getTimeout() const
{
  return _client->getTimeout();
}

/* ************************************************************************** */
// PRIVATE

ClientEventHandler::Result ClientEventHandler::_handlePollInEvent()
{
  const bool alive = _client->receive();
  if (alive) {
    updateLastActivity();
    _clientStateMachine();

    if (_client->hasDataToSend()) {
      SocketManager::getInstance().enablePollout(_client->getFd());
      _log.info() << "Pollout enabled\n";
    }
  } else {
    return Disconnect;
  }
  return Alive;
}

ClientEventHandler::Result ClientEventHandler::_handlePollOutEvent()
{
  _clientStateMachine();

  if (_client->hasDataToSend()) {
    _sending = true;
    const bool alive = _client->sendTo();
    updateLastActivity();

    if (alive) {
      if (!_client->hasDataToSend() && _client->getInBuff().isEmpty()) {
        _log.info() << "Pollout disabled\n";
        SocketManager::getInstance().disablePollout(_client->getFd());
        _sending = false;
      }
    } else {
      return Disconnect;
    }
  }
  return Alive;
}

void ClientEventHandler::_clientStateMachine()
{
  StateHandler<Client>& handler = _client->getStateHandler();
  const Response& response = _client->getResponse();

  if (handler.isDone() && !_client->getInBuff().isEmpty() &&
      !_client->closeConnection() && response.getStatusCode().is2xxCode()) {
    _log.info() << _client->getInBuff() << '\n';
    _client->prepareForNewRequest();
    _cgiEventHandlerAdded = false;
  }

  handler.setStateHasChanged(true);
  while (!handler.isDone() && handler.stateHasChanged()) {
    handler.setStateHasChanged(false);
    handler.getState()->run();
  }

  if (_client->getCgiContext() != FT_NULLPTR && !_cgiEventHandlerAdded) {
    _addCgiEventHandler();
  }
}

void ClientEventHandler::_handleException()
{
  if (!_sending) {
    const char* const msg = http::minResponse500;
    const std::size_t msgLen = std::strlen(msg);
    (void)send(getFd(), msg, msgLen, 0);
  }
}

void ClientEventHandler::_addCgiEventHandler()
{
  CgiContext& cgiContext = *_client->getCgiContext();
  // SocketManager& socketManager = SocketManager::getInstance();
  EventManager& eventManager = EventManager::getInstance();

  cgiContext.getPipeClientToCgi().init();
  cgiContext.getPipeCgiToClient().init();

  const int fdClientToCgi = cgiContext.getPipeClientToCgi().getWriteFd();
  const int fdCgiToClient = cgiContext.getPipeCgiToClient().getReadFd();

  ft::shared_ptr<CgiWriteEventHandler> cgiWriteHandler =
    ft::make_shared<CgiWriteEventHandler>(fdClientToCgi, _client);
  ft::shared_ptr<CgiReadEventHandler> cgiReadHandler =
    ft::make_shared<CgiReadEventHandler>(fdCgiToClient, _client);

  eventManager.addCgiHandler(ft::move(cgiWriteHandler));
  eventManager.addCgiHandler(ft::move(cgiReadHandler));

  // enable poll
  SocketManager::getInstance().enablePollout(fdClientToCgi);

  _cgiEventHandlerAdded = true;
}
