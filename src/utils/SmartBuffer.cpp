#include "SmartBuffer.hpp"

#include <libftpp/expected.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/FileBuffer.hpp>
#include <utils/IBuffer.hpp>

#include <cstddef>
#include <exception>
#include <new>
#include <string>

/* ************************************************************************** */
// INIT

const char* const SmartBuffer::errAllocBuffer =
  "SbException: failed to initialize buffer";

/* ************************************************************************** */
// PUBLIC

SmartBuffer::SmartBuffer()
  : _buffer(new FileBuffer())
  , _usesFile(true)
{
}

// Throwing versions

char SmartBuffer::get()
{
  return _buffer->get();
}

char SmartBuffer::peek()
{
  return _buffer->peek();
}

void SmartBuffer::seek(std::size_t pos)
{
  _buffer->seek(pos);
}

void SmartBuffer::append(const std::string& data)
{
  if (!_usesFile && _fileNeeded(data.size())) {
    _switchToFileBuffer();
  }
  _buffer->append(data);
}

void SmartBuffer::append(const RawBytes& buffer, std::size_t bytes)
{
  if (!_usesFile && _fileNeeded(bytes)) {
    _switchToFileBuffer();
  }
  _buffer->append(buffer, bytes);
}

void SmartBuffer::removeFront(std::size_t bytes)
{
  // todo switch to memory buffer if possible
  _buffer->removeFront(bytes);
}

std::string SmartBuffer::consumeFront(std::size_t bytes)
{
  // todo switch to memory buffer if possible
  return _buffer->consumeFront(bytes);
}

SmartBuffer::RawBytes SmartBuffer::consumeAll()
{
  // todo switch to memory buffer
  return _buffer->consumeAll();
}

std::string SmartBuffer::getStr(std::size_t start, std::size_t bytes)
{
  return _buffer->getStr(start, bytes);
}

SmartBuffer::RawBytes SmartBuffer::getRawBytes(std::size_t start,
                                               std::size_t bytes)
{
  return _buffer->getRawBytes(start, bytes);
}

void SmartBuffer::replace(RawBytes& rawData)
{
  // todo maybe check size first
  _buffer->replace(rawData);
}

// Non-throwing versions

IBuffer::ExpectChr SmartBuffer::get(std::nothrow_t /*unused*/)
{
  try {
    return get();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectChr SmartBuffer::peek(std::nothrow_t /*unused*/)
{
  try {
    return peek();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid SmartBuffer::seek(std::size_t pos,
                                      std::nothrow_t /*unused*/)
{
  try {
    seek(pos);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid SmartBuffer::append(const std::string& data,
                                        std::nothrow_t /*unused*/)
{
  try {
    append(data);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid SmartBuffer::append(const RawBytes& buffer,
                                        std::size_t bytes,
                                        std::nothrow_t /*unused*/)
{
  try {
    append(buffer, bytes);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid SmartBuffer::removeFront(std::size_t bytes,
                                             std::nothrow_t /*unused*/)
{
  try {
    removeFront(bytes);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectStr SmartBuffer::consumeFront(std::size_t bytes,
                                             std::nothrow_t /*unused*/)
{
  try {
    return consumeFront(bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw SmartBuffer::consumeAll(std::nothrow_t /*unused*/)
{
  try {
    return consumeAll();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectStr SmartBuffer::getStr(std::size_t start,
                                       std::size_t bytes,
                                       std::nothrow_t /*unused*/)
{
  try {
    return getStr(start, bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw SmartBuffer::getRawBytes(std::size_t start,
                                            std::size_t bytes,
                                            std::nothrow_t /*unused*/)
{
  try {
    return getRawBytes(start, bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid SmartBuffer::replace(RawBytes& rawData,
                                         std::nothrow_t /*unused*/)
{
  try {
    replace(rawData);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
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

bool SmartBuffer::_fileNeeded(std::size_t newBytes)
{
  return _buffer->size() + newBytes >= _thresholdMemoryBuffer;
}

void SmartBuffer::_switchToFileBuffer()
{
  ft::unique_ptr<FileBuffer> newBuffer = ft::make_unique<FileBuffer>();
  RawBytes data = _buffer->consumeAll();
  newBuffer->replace(data);
  _buffer = ft::move(newBuffer);
  _usesFile = true;
}
