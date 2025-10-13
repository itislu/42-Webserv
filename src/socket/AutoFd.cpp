#include "AutoFd.hpp"
#include <unistd.h>

AutoFd::AutoFd(int fdes)
  : _fd(fdes)
{
}

AutoFd::~AutoFd()
{
  if (_fd >= 0) {
    ::close(_fd);
  }
}

void AutoFd::set(int fdes)
{
  if (_fd >= 0) {
    close(_fd);
  }
  _fd = fdes;
}

int AutoFd::get() const
{
  return _fd;
}
