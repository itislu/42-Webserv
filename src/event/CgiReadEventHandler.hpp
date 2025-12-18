#pragma once
#ifndef CGI_READ_EVENT_HANDLER_HPP
#define CGI_READ_EVENT_HANDLER_HPP

#include <event/EventHandler.hpp>
#include <libftpp/memory.hpp>
#include <utils/logger/Logger.hpp>

#include <string>

class Client;

/* ************************************************************************** */
class CgiReadEventHandler : public EventHandler
{
public:
  explicit CgiReadEventHandler(int fdes, ft::shared_ptr<Client> client);

  Result handleEvent(unsigned revents);
  Result onTimeout();
  long getTimeout() const;
  std::string logName() const;

private:
  Result _handlePollInEvent();
  Result _handlePollHupOrErr();

  static Logger& _log;
  ft::shared_ptr<Client> _client;
};

#endif
