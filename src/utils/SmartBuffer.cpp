#include "SmartBuffer.hpp"

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/FileBuffer.hpp>
#include <utils/IBuffer.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
// INIT

const char* const SmartBuffer::errAllocBuffer =
  "SbException: failed to initialize buffer";

/* ************************************************************************** */
// PUBLIC

SmartBuffer::SmartBuffer()
  : _usesFile(true)
{
  ft::unique_ptr<FileBuffer> newBuffer = ft::make_unique<FileBuffer>();
  _buffer = ft::move(newBuffer);
}

IBuffer::ExpectChr SmartBuffer::get()
{
  return _buffer->get();
}

IBuffer::ExpectChr SmartBuffer::peek()
{
  return _buffer->peek();
}

IBuffer::ExpectVoid SmartBuffer::seek(std::size_t pos)
{
  return _buffer->seek(pos);
}

IBuffer::ExpectVoid SmartBuffer::append(const std::string& data)
{
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
  // todo switch to memory buffer if possible
  return _buffer->removeFront(bytes);
}

IBuffer::ExpectStr SmartBuffer::consumeFront(std::size_t bytes)
{
  // todo switch to memory buffer if possible
  return _buffer->consumeFront(bytes);
}

IBuffer::ExpectRaw SmartBuffer::consumeAll()
{
  // todo switch to memory buffer
  return _buffer->consumeAll();
}

IBuffer::ExpectStr SmartBuffer::getStr(std::size_t start, std::size_t bytes)
{
  return _buffer->getStr(start, bytes);
}

IBuffer::ExpectRaw SmartBuffer::getRawBytes(std::size_t start,
                                            std::size_t bytes)
{
  return _buffer->getRawBytes(start, bytes);
}

IBuffer::ExpectVoid SmartBuffer::replace(RawBytes& rawData)
{
  // todo maybe check size first
  return _buffer->replace(rawData);
}

bool SmartBuffer::isEmpty() const
{
  return _buffer->isEmpty();
}

std::size_t SmartBuffer::size() const
{
  return _buffer->size();
}

void SmartBuffer::print()
{
  _buffer->print();
}

/* ************************************************************************** */
// PRIVATE

bool SmartBuffer::_fileNeeded(long newBytes)
{
  return _buffer->size() + newBytes >= _thresholdMemoryBuffer;
}

IBuffer::ExpectVoid SmartBuffer::_switchToFileBuffer()
{
  ft::unique_ptr<FileBuffer> newBuffer = ft::make_unique<FileBuffer>();
  ExpectRaw res = _buffer->consumeAll();
  if (!res.has_value()) {
    return handleUnexpected(res.error().what());
  }

  const ExpectVoid resVoid = newBuffer->replace(*res);
  if (!resVoid.has_value()) {
    return resVoid;
  }
  _buffer = ft::move(newBuffer);
  _usesFile = true;
  return resVoid;
}
