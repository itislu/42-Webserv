#include "IInBuffer.hpp"

#include <libftpp/expected.hpp>
#include <utils/buffer/IBuffer.hpp>

#include <cstddef>
#include <exception>
#include <new>
#include <ostream>

/* ************************************************************************** */
// PUBLIC

// Non-throwing versions

IBuffer::ExpectChr IInBuffer::get(std::nothrow_t /*unused*/)
{
  try {
    return get();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectChr IInBuffer::peek(std::nothrow_t /*unused*/)
{
  try {
    return peek();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid IInBuffer::seek(std::size_t pos, std::nothrow_t /*unused*/)
{
  try {
    seek(pos);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectPos IInBuffer::pos(std::nothrow_t /*unused*/)
{
  try {
    return pos();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectStr IInBuffer::consumeFront(std::size_t bytes,
                                           std::nothrow_t /*unused*/)
{
  try {
    return consumeFront(bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw IInBuffer::consumeRawFront(std::size_t bytes,
                                              std::nothrow_t /*unused*/)
{
  try {
    return consumeRawFront(bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw IInBuffer::consumeAll(std::nothrow_t /*unused*/)
{
  try {
    return consumeAll();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectStr IInBuffer::getStr(std::size_t start,
                                     std::size_t bytes,
                                     std::nothrow_t /*unused*/)
{
  try {
    return getStr(start, bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw IInBuffer::getRawBytes(std::size_t start,
                                          std::size_t bytes,
                                          std::nothrow_t /*unused*/)
{
  try {
    return getRawBytes(start, bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

std::ostream& operator<<(std::ostream& out, IInBuffer& buffer)
{
  const std::size_t oldpos = buffer.pos();
  std::size_t pos = oldpos;

  out << "'";
  while (pos < buffer.size()) {
    const char chr = buffer.get();
    // printEscapedChar(out, chr);
    out << chr;
    ++pos;
  }
  out << "'\n";
  buffer.seek(oldpos);

  return out;
}
