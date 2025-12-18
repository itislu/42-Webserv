#pragma once
#ifndef CGI_WRITE_EVENT_HANDLER_HPP
#define CGI_WRITE_EVENT_HANDLER_HPP

#include <event/EventHandler.hpp>
#include <libftpp/memory.hpp>
#include <utils/logger/Logger.hpp>

#include <string>

class Client;

/* ************************************************************************** */
class CgiWriteEventHandler : public EventHandler
{
public:
  explicit CgiWriteEventHandler(int fdes, ft::shared_ptr<Client> client);

  Result handleEvent(unsigned revents);
  Result onTimeout();
  long getTimeout() const;
  std::string logName() const;

private:
  Result _handlePollOutEvent();

  static Logger& _log;
  ft::shared_ptr<Client> _client;
};

#endif
