#pragma once
#ifndef CLIENT_EVENT_HANDLER_HPP
#define CLIENT_EVENT_HANDLER_HPP

#include <event/EventHandler.hpp>
#include <libftpp/memory.hpp>
#include <utils/logger/Logger.hpp>

#include <string>

class Client;

/* ************************************************************************** */
class ClientEventHandler : public EventHandler
{
public:
  explicit ClientEventHandler(int fdes, ft::shared_ptr<Client> client);

  Result handleEvent(unsigned revents);
  Result onTimeout();
  long getTimeout() const;
  std::string logName() const;

private:
  Result _handlePollInEvent();
  Result _handlePollOutEvent();
  void _clientStateMachine();
  void _handleException();
  void _addCgiEventHandler();
  void _prepareForNewRequest();
  void _sendMinResponse(const char* msg);

  static Logger& _log;
  ft::shared_ptr<Client> _client;

  bool _cgiEventHandlerAdded;
  bool _sending;
};

#endif
