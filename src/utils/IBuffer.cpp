#include "IBuffer.hpp"
#include "libftpp/utility.hpp"

/* ************************************************************************** */
// Exceptions

/* ************************************************************************** */
// PUBLIC
IBuffer::BufferException::BufferException()
  : _message(FT_NULLPTR)
{
}

IBuffer::BufferException::BufferException(const char* message)
  : _message(message)
{
}

IBuffer::BufferException::BufferException(const IBuffer::BufferException& other)
  : _message(other._message)
{
}

// NOLINTBEGIN(bugprone-unhandled-self-assignment)
IBuffer::BufferException& IBuffer::BufferException::operator=(
  const BufferException& other)
{
  _message = other._message;
  return *this;
}
// NOLINTEND(bugprone-unhandled-self-assignment)

const char* IBuffer::BufferException::what() const throw()
{
  return _message;
}
