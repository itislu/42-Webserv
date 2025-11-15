#include "IBuffer.hpp"

/* ************************************************************************** */
// PUBLIC

IBuffer::IBuffer() {}

IBuffer::~IBuffer() {}

IBuffer::IBuffer(const IBuffer& other)
{
  *this = other;
}

IBuffer& IBuffer::operator=(const IBuffer& other)
{
  if (this != &other) {
    // copy logic
  }
  return *this;
}

/* ************************************************************************** */
// Exceptions

/* ************************************************************************** */
// PUBLIC
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
