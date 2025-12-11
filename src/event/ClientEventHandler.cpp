#include "ClientEventHandler.hpp"

#include <client/Client.hpp>
#include <event/EventHandler.hpp>
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
{
}

ClientEventHandler::Result ClientEventHandler::handleEvent(unsigned revents)
try {
  updateLastActivity();
  if (_client == FT_NULLPTR) {
    return Disconnect;
  }
  if (isPollInEvent(revents)) {
    return _checkPollInEvent();
  }
  if (isPollOutEvent(revents)) {
    return _checkPollOutEvent();
  }
  if (isPollHupEvent(revents) || isPollErrEvent(revents)) {
    return Disconnect;
  }
  return Disconnect;
} catch (const std::exception& e) {
  _log.error() << "CgiWriteEventHandler exception: " << e.what() << '\n';
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  _handleException();
  return Disconnect;
}

long ClientEventHandler::getTimeout() const
{
  return _client->getTimeout();
}

/* ************************************************************************** */
// PRIVATE

ClientEventHandler::Result ClientEventHandler::_checkPollInEvent()
{
  const Result result = _receiveFromClient();
  if (result == Alive) {
    _clientStateMachine();
  }

  if (_client->hasDataToSend()) {
    SocketManager::getInstance().enablePollout(_client->getFd());
    _log.info() << "Pollout enabled\n";
  }
  return result;
}

ClientEventHandler::Result ClientEventHandler::_checkPollOutEvent()
{
  Result result = Alive;

  _clientStateMachine();

  if (_client->hasDataToSend()) {
    result = _sendToClient();
  }
  return result;
}

ClientEventHandler::Result ClientEventHandler::_sendToClient()
{
  const bool alive = _client->sendTo();
  if (alive && !_client->hasDataToSend() && _client->getInBuff().isEmpty()) {
    _log.info() << "Pollout disabled\n";
    SocketManager::getInstance().disablePollout(_client->getFd());
  }

  if (!alive) {
    return Disconnect;
  }
  return Alive;
}

ClientEventHandler::Result ClientEventHandler::_receiveFromClient()
{
  const bool alive = _client->receive();
  if (_client->getStateHandler().isDone() && !_client->getInBuff().isEmpty()) {
    _client->prepareForNewRequest();
  }

  if (!alive) {
    return Disconnect;
  }
  return Alive;
}

void ClientEventHandler::_clientStateMachine()
{
  StateHandler<Client>& handler = _client->getStateHandler();

  handler.setStateHasChanged(true);
  while (!handler.isDone() && handler.stateHasChanged()) {
    handler.setStateHasChanged(false);
    handler.getState()->run();
  }
}

void ClientEventHandler::_handleException()
{
  /**  //todo
   * should send response message only if nothing has been sent yet.
   * tricky for piped requests
   */
  const char* const msg = http::minResponse500;
  const std::size_t msgLen = std::strlen(msg);
  (void)send(getFd(), msg, msgLen, 0);
}
