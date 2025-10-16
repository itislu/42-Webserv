#include "Buffer.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <sys/types.h>

/* ************************************************************************** */
// PUBLIC

Buffer::Buffer() {}

Buffer::Buffer(std::size_t size)
{
  _buff.reserve(size);
}

void Buffer::add(const std::string& str)
{
  _buff.insert(_buff.end(), str.begin(), str.end());
}

void Buffer::add(const Container& buffer)
{
  _buff.insert(_buff.end(), buffer.begin(), buffer.end());
}

void Buffer::remove(ssize_t bytes)
{
  _buff.erase(_buff.begin(), _buff.begin() + bytes);
}

void Buffer::remove(size_t bytes)
{
  remove(static_cast<ssize_t>(bytes));
}

std::size_t Buffer::getSize() const
{
  return _buff.size();
}

unsigned char* Buffer::data()
{
  return _buff.data();
}

Buffer::iterator Buffer::begin()
{
  return _buff.begin();
}

Buffer::iterator Buffer::end()
{
  return _buff.end();
}

Buffer::iterator Buffer::getIterAt(long offset)
{
  if (offset <= 0) {
    return _buff.begin();
  }
  if (offset >= static_cast<long>(_buff.size())) {
    return _buff.end();
  }
  return _buff.begin() + offset;
}

std::string Buffer::getString(long fromIndex, long toIndex) const
{
  if (toIndex < fromIndex) {
    throw std::invalid_argument("toIndex must be bigger or equal to fromIndex");
  }
  if (fromIndex < 0 || toIndex < 0) {
    throw std::invalid_argument("index must be >= 0");
  }
  return std::string(_buff.begin() + fromIndex, _buff.begin() + toIndex);
}

bool Buffer::isEmpty() const
{
  return _buff.empty();
}
