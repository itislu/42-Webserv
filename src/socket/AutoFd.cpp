#include "AutoFd.hpp"

#include <libftpp/movable.hpp>

#include <set>
#include <unistd.h>

AutoFd::AutoFd(int fdes)
  : _fd(fdes)
{
}

AutoFd::AutoFd(ft::rvalue<AutoFd>& other)
  : _fd(other._fd)
{
  other._fd = -1;
  _subscribers.swap(other._subscribers);
}

AutoFd& AutoFd::operator=(ft::rvalue<AutoFd>& other)
{
  if (&other == this) {
    return *this;
  }
  _close();

  _fd = other._fd;
  other._fd = -1;

  _subscribers.swap(other._subscribers);
  other._subscribers.clear();

  return *this;
}

AutoFd::~AutoFd()
{
  _close();
}

void AutoFd::set(int fdes)
{
  if (fdes == _fd) {
    return;
  }

  _close();
  _fd = fdes;
}

int AutoFd::get() const
{
  return _fd;
}

void AutoFd::subscribe(AutoFdSubscriber& subscriber)
{
  _subscribers.insert(&subscriber);
}

void AutoFd::unsubscribe(AutoFdSubscriber& subscriber)
{
  _subscribers.erase(&subscriber);
}

void AutoFd::_close()
{
  if (_fd >= 0) {
    _notifyClose();
    ::close(_fd);
  }
}

void AutoFd::_notifyClose()
{
  for (std::set<AutoFdSubscriber*>::iterator it = _subscribers.begin();
       it != _subscribers.end();
       ++it) {
    (*it)->onClose(_fd);
  }
}
