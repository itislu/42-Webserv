#include "MemoryBuffer.hpp"

#include <utils/buffer/IBuffer.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <ios>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

/* ************************************************************************** */
// INIT

const char* const MemoryBuffer::errOpen = "MemoryBuffer exception: open failed";
const char* const MemoryBuffer::errOutOfRange =
  "MemoryBuffer exception: tried to access out of range";
const char* const MemoryBuffer::errBufferEmpty =
  "MemoryBuffer exception: buffer is empty";
const char* const MemoryBuffer::errWrite =
  "MemoryBuffer exception: write failed";

/* ************************************************************************** */
// PUBLIC

MemoryBuffer::MemoryBuffer()
  : _pos(0)
{
  _data.reserve(_startSize);
}

MemoryBuffer::MemoryBuffer(const std::string& data)
  : _pos(0)
{
  MemoryBuffer::append(data);
}

MemoryBuffer::MemoryBuffer(const RawBytes& buffer, std::size_t bytes)
  : _pos(0)
{
  MemoryBuffer::append(buffer, bytes);
}

MemoryBuffer::~MemoryBuffer() {}

char MemoryBuffer::get()
{
  if (_pos >= _data.size()) {
    throw BufferException(errOutOfRange);
  }
  const char currChar = _data[_pos];
  _pos++;
  return currChar;
}

char MemoryBuffer::peek()
{
  if (_pos >= _data.size()) {
    throw BufferException(errOutOfRange);
  }
  return _data[_pos];
}

void MemoryBuffer::seek(std::size_t pos)
{
  if (pos > _data.size()) {
    if (_data.empty()) {
      throw BufferException(errBufferEmpty);
    }
    throw BufferException(errOutOfRange);
  }
  if (_data.empty()) {
    return;
  }
  _pos = pos;
}

std::size_t MemoryBuffer::pos()
{
  return _pos;
}

std::string MemoryBuffer::consumeFront(std::size_t bytes)
{
  return _consumeFront<std::string>(bytes);
}

MemoryBuffer::RawBytes MemoryBuffer::consumeRawFront(std::size_t bytes)
{
  return _consumeFront<RawBytes>(bytes);
}

MemoryBuffer::RawBytes MemoryBuffer::consumeAll()
{
  return _consumeFront<RawBytes>(_data.size());
}

std::string MemoryBuffer::getStr(std::size_t start, std::size_t bytes)
{
  return _getData<std::string>(start, bytes);
}

MemoryBuffer::RawBytes MemoryBuffer::getRawBytes(std::size_t start,
                                                 std::size_t bytes)
{
  return _getData<RawBytes>(start, bytes);
}

void MemoryBuffer::append(const std::string& data)
{
  _data.insert(_data.end(), data.begin(), data.end());
}

void MemoryBuffer::append(const RawBytes& buffer, std::size_t bytes)
{
  assert(bytes <= buffer.size());
  _data.insert(_data.end(),
               buffer.begin(),
               buffer.begin() + static_cast<RawBytes::difference_type>(bytes));
}

void MemoryBuffer::removeFront(std::size_t bytes)
{
  // seek to check buffer limits and set _pos
  seek(bytes);
  _data.erase(_data.begin(),
              _data.begin() + static_cast<RawBytes::difference_type>(_pos));
  _pos = 0;
}

void MemoryBuffer::replace(RawBytes& raw)
{
  _data.swap(raw);
  _pos = 0;
}

void MemoryBuffer::moveBufferToFile(const std::string& filepath)
{
  std::ofstream ofs(filepath.c_str(),
                    std::ios::out | std::ios::binary | std::ios::trunc);

  if (!ofs.is_open()) {
    throw BufferException(errOpen);
  }

  if (!_data.empty()) {
    ofs.write(_data.data(), static_cast<std::streamsize>(_data.size()));
    if (!ofs.good()) {
      throw BufferException(errWrite);
    }
  }

  _data.clear();
  _pos = 0;
}

bool MemoryBuffer::isEmpty() const
{
  return _data.empty();
}

std::size_t MemoryBuffer::size() const
{
  return _data.size();
}

ssize_t MemoryBuffer::send(int fdes, std::size_t bytes)
{
  bytes = std::min(bytes, size());
  const ssize_t bytesSent = ::send(fdes, _data.data(), bytes, 0);
  if (bytesSent > 0) {
    removeFront(bytesSent);
  }
  return bytesSent;
}

/* ************************************************************************** */
// PRIVATE

// Template can be in source file if only used here.
template<typename ContigContainer>
ContigContainer MemoryBuffer::_consumeFront(std::size_t bytes)
{
  // read bytes from the beginning
  const ContigContainer front = _getData<ContigContainer>(0, bytes);
  assert(front.size() == bytes); // Unexpected EOF should not happen

  // remove read bytes
  removeFront(front.size());
  return front;
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ContigContainer MemoryBuffer::_getData(std::size_t start, std::size_t bytes)
{
  if (start >= _data.size() || bytes > _data.size() - start) {
    if (_data.empty()) {
      throw BufferException(errBufferEmpty);
    }
    throw BufferException(errOutOfRange);
  }
  if (bytes == 0) {
    return ContigContainer();
  }
  ContigContainer front(
    _data.begin() +
      static_cast<typename ContigContainer::difference_type>(start),
    _data.begin() +
      static_cast<typename ContigContainer::difference_type>(start + bytes));
  return front;
}
