#include "SmartBuffer.hpp"

#include <libftpp/expected.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/FileBuffer.hpp>
#include <utils/IBuffer.hpp>

#include <cassert>
#include <cstddef>
#include <string>

/* ************************************************************************** */
// INIT

const char* const SmartBuffer::errAllocBuffer =
  "SbException: failed to initialize buffer";

/* ************************************************************************** */
// PUBLIC

SmartBuffer::SmartBuffer()
  : _usesFile(false)
{
}

IBuffer::ExpectChr SmartBuffer::get()
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  return _buffer->get();
}

IBuffer::ExpectChr SmartBuffer::peek()
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  return _buffer->peek();
}

IBuffer::ExpectVoid SmartBuffer::seek(std::size_t pos)
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  return _buffer->seek(pos);
}

IBuffer::ExpectVoid SmartBuffer::append(const std::string& data)
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  if (!_usesFile && _fileNeeded(static_cast<long>(data.size()))) {
    const ExpectVoid res = _switchToFileBuffer();
    if (!res.has_value()) {
      return res;
    }
  }
  return _buffer->append(data);
}

IBuffer::ExpectVoid SmartBuffer::append(const RawBytes& buffer, long bytes)
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  if (!_usesFile && _fileNeeded(bytes)) {
    const ExpectVoid res = _switchToFileBuffer();
    if (!res.has_value()) {
      return res;
    }
  }
  return _buffer->append(buffer, bytes);
}

IBuffer::ExpectVoid SmartBuffer::removeFront(std::size_t bytes)
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  return _buffer->removeFront(bytes);
}

IBuffer::ExpectStr SmartBuffer::consumeFront(std::size_t bytes)
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  return _buffer->consumeFront(bytes);
}

IBuffer::ExpectRaw SmartBuffer::consumeAll()
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  return _buffer->consumeAll();
}

IBuffer::ExpectVoid SmartBuffer::replace(RawBytes& rawData)
{
  _checkBuffer();
  if (_buffer == FT_NULLPTR) {
    return ft::unexpected<BufferException>(errAllocBuffer);
  }
  return _buffer->replace(rawData);
}

bool SmartBuffer::isEmpty() const
{
  if (_buffer == FT_NULLPTR) {
    return true;
  }
  return _buffer->isEmpty();
}

std::size_t SmartBuffer::size() const
{
  if (_buffer == FT_NULLPTR) {
    return 0;
  }
  return _buffer->size();
}

/* ************************************************************************** */
// PRIVATE

void SmartBuffer::_checkBuffer()
{
  if (_buffer == FT_NULLPTR) {
    // todo should first be MemoryBuffer
    _buffer = ft::make_shared<FileBuffer>();
    _usesFile = true;
  }
}

bool SmartBuffer::_fileNeeded(long newBytes)
{
  return _buffer->size() + newBytes >= _thresholdMemoryBuffer;
}

IBuffer::ExpectVoid SmartBuffer::_switchToFileBuffer()
{
  ExpectRaw res = _buffer->consumeAll();
  _buffer.reset();
  if (!res.has_value()) {
    return ft::unexpected<BufferException>(res.error().what());
  }
  _buffer = ft::make_shared<FileBuffer>();

  const ExpectVoid resVoid = _buffer->replace(*res);
  if (!resVoid.has_value()) {
    _buffer.reset();
    return resVoid;
  }
  _usesFile = true;
  return resVoid;
}
