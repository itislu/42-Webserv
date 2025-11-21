#include "IBuffer.hpp"

#include "libftpp/expected.hpp"
#include "libftpp/utility.hpp"
#include "utils/printUtils.hpp"

#include <cstddef>
#include <ostream>

/* ************************************************************************** */
// PUBLIC
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
// Exceptions
IBuffer::BufferException::BufferException()
  : _message(FT_NULLPTR)
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
