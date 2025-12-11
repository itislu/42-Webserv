#include "EventHandler.hpp"

#include <client/TimeStamp.hpp>

#include <sys/poll.h>

/* ************************************************************************** */
// PUBLIC

EventHandler::EventHandler(int fdes)
  : _fdes(fdes)
{
}

const TimeStamp& EventHandler::getLastActivity() const
{
  return _lastActivity;
}

/* ************************************************************************** */
// PROTECTED

void EventHandler::updateLastActivity()
{
  _lastActivity.setTime(TimeStamp::now());
}

bool EventHandler::isPollInEvent(unsigned events)
{
  return (events & POLLIN) != 0;
}

bool EventHandler::isPollOutEvent(unsigned events)
{
  return (events & POLLOUT) != 0;
}

bool EventHandler::isPollHupEvent(unsigned events)
{
  return (events & POLLHUP) != 0;
}

bool EventHandler::isPollErrEvent(unsigned events)
{
  return (events & POLLERR) != 0;
}

/* ************************************************************************** */
// PRIVATE

int EventHandler::getFd() const
{
  return _fdes;
}
