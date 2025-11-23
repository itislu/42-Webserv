#include "IBuffer.hpp"
#include "libftpp/expected.hpp"

/* ************************************************************************** */
// Exceptions
IBuffer::IBuffer()
  : _noThrow(false)
{
}

void IBuffer::setNoThrow(bool value)
{
  _noThrow = value;
}

ft::unexpected<IBuffer::BufferException> IBuffer::handleUnexpected(
  const char* message) const
{
  if (_noThrow) {
    return ft::unexpected<BufferException>(message);
  }
  throw BufferException(message);
}

/* ************************************************************************** */
// PUBLIC
IBuffer::BufferException::BufferException()
  : _message("Success")
{
}

IBuffer::BufferException::BufferException(const char* message)
  : _message(message)
{
}

IBuffer::BufferException::BufferException(
  const IBuffer::BufferException& other) throw()
  : _message(other._message)
{
}

// NOLINTBEGIN(bugprone-unhandled-self-assignment)
IBuffer::BufferException& IBuffer::BufferException::operator=(
  const BufferException& other) throw()
{
  _message = other._message;
  return *this;
}
// NOLINTEND(bugprone-unhandled-self-assignment)

const char* IBuffer::BufferException::what() const throw()
{
  return _message;
}
