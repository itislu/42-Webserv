#pragma once
#ifndef CGI_WRITE_EVENT_HANDLER_HPP
#define CGI_WRITE_EVENT_HANDLER_HPP

#include <event/EventHandler.hpp>
#include <utils/logger/Logger.hpp>

class Client;

/* ************************************************************************** */
class CgiWriteEventHandler : public EventHandler
{
public:
  explicit CgiWriteEventHandler(int fdes, Client* client);

  Result handleEvent(unsigned revents);
  long getTimeout() const;

private:
  static Logger& _log;
  Client* _client;
};

#endif
