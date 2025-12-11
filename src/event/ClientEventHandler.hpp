#pragma once
#ifndef CLIENT_EVENT_HANDLER_HPP
#define CLIENT_EVENT_HANDLER_HPP

#include <event/EventHandler.hpp>
#include <libftpp/memory.hpp>
#include <utils/logger/Logger.hpp>

class Client;

/* ************************************************************************** */
class ClientEventHandler : public EventHandler
{
public:
  explicit ClientEventHandler(int fdes, ft::shared_ptr<Client>);

  Result handleEvent(unsigned revents);
  long getTimeout() const;

private:
  Result _checkPollInEvent();
  Result _checkPollOutEvent();
  Result _receiveFromClient();
  Result _sendToClient();
  void _clientStateMachine();
  void _handleException();

  static Logger& _log;
  ft::shared_ptr<Client> _client;
};

#endif
