#pragma once
#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <client/TimeStamp.hpp>

#include <string>

/* ************************************************************************** */
class EventHandler
{
public:
  enum Result
  {
    Alive,
    Disconnect,
    TimeoutExtended
  };

  explicit EventHandler(int fdes);
  virtual ~EventHandler() {}

  virtual Result handleEvent(unsigned revents) = 0;
  virtual Result onTimeout() = 0;
  virtual long getTimeout() const = 0;
  virtual std::string logName() const = 0;

  const TimeStamp& getLastActivity() const;
  int getFd() const;

protected:
  void updateLastActivity();

  static bool isPollInEvent(unsigned revents);
  static bool isPollOutEvent(unsigned revents);
  static bool isPollHupEvent(unsigned revents);
  static bool isPollErrEvent(unsigned revents);

private:
  EventHandler(const EventHandler&);
  EventHandler& operator=(const EventHandler&);

  int _fdes;
  TimeStamp _lastActivity;
};

#endif
