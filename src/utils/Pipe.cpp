#include "Pipe.hpp"

#include <fcntl.h> //fcntl()
#include <unistd.h>

/* ************************************************************************** */
// PUBLIC

Pipe::Pipe()
  : _pipefd()
{
  _pipefd[0] = -1;
  _pipefd[1] = -1;
}

Pipe::~Pipe()
{
  close();
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
void Pipe::init()
{
  if (_pipefd[0] != -1 || _pipefd[1] != -1) {
    close();
  }
  if (pipe(_pipefd) == -1) {
    throw PipeException();
  }

  // make fds non-blocking
  // todo non block ?
  // _setFdNonBlocking(_pipefd[0]);
  // _setFdNonBlocking(_pipefd[1]);
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

void Pipe::close()
{
  closeRead();
  closeWrite();
}

void Pipe::closeRead()
{
  if (_pipefd[0] != -1) {
    ::close(_pipefd[0]);
    _pipefd[0] = -1;
  }
}

void Pipe::closeWrite()
{
  if (_pipefd[1] != -1) {
    ::close(_pipefd[1]);
    _pipefd[1] = -1;
  }
}

int Pipe::getReadFd() const
{
  return _pipefd[0];
}

int Pipe::getWriteFd() const
{
  return _pipefd[1];
}

/* ************************************************************************** */
// PRIVATE
void Pipe::_setFdNonBlocking(int fdes)
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg): POSIX C API.
  if (fcntl(fdes, F_SETFL, O_NONBLOCK) < 0) {
    throw PipeException();
  }
}

/* ************************************************************************** */
// PUBLIC
const char* Pipe::PipeException::what() const throw()
{
  return "PipeException: pipe failed";
}
