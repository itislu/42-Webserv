#include "IBuffer.hpp"

#include <cstddef>
#include <exception>
#include <ostream>

/* ************************************************************************** */
// Exceptions

std::ostream& operator<<(std::ostream& out, IBuffer& buffer)
{
  const std::size_t oldpos = buffer.pos();
  std::size_t pos = oldpos;

  out << "'";
  while (pos + 1 < buffer.size()) {
    const IBuffer::ExpectChr expectChr = buffer.get();
    const char chr = *expectChr;
    // printEscapedChar(out, chr);
    out << chr;
    ++pos;
  }
  out << "'\n";
  buffer.seek(oldpos);

  return out;
}

/* ************************************************************************** */
// PUBLIC
IBuffer::BufferException::BufferException(const char* message)
  : _message(message)
{
}

IBuffer::BufferException::BufferException(const std::exception& exception)
  : _message(exception.what())
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
