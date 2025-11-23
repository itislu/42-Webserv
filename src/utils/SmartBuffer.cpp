#include "SmartBuffer.hpp"

#include <libftpp/expected.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/FileBuffer.hpp>
#include <utils/IBuffer.hpp>

#include <cstddef>
#include <new>
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

char SmartBuffer::get()
{
  return _buffer->get();
}

IBuffer::ExpectChr SmartBuffer::get(std::nothrow_t /*unused*/)
{
  try {
    return get();
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

char SmartBuffer::peek()
{
  return _buffer->peek();
}

IBuffer::ExpectChr SmartBuffer::peek(std::nothrow_t /*unused*/)
{
  try {
    return peek();
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

void SmartBuffer::seek(std::size_t pos)
{
  _buffer->seek(pos);
}

IBuffer::ExpectVoid SmartBuffer::seek(std::size_t pos,
                                      std::nothrow_t /*unused*/)
{
  try {
    seek(pos);
    return ExpectVoid();
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

void SmartBuffer::append(const std::string& data)
{
  if (!_usesFile && _fileNeeded(static_cast<long>(data.size()))) {
    _switchToFileBuffer();
  }
  _buffer->append(data);
}

IBuffer::ExpectVoid SmartBuffer::append(const std::string& data,
                                        std::nothrow_t /*unused*/)
{
  try {
    append(data);
    return ExpectVoid();
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

void SmartBuffer::append(const RawBytes& buffer, long bytes)
{
  if (!_usesFile && _fileNeeded(bytes)) {
    _switchToFileBuffer();
  }
  _buffer->append(buffer, bytes);
}

IBuffer::ExpectVoid SmartBuffer::append(const RawBytes& buffer,
                                        long bytes,
                                        std::nothrow_t /*unused*/)
{
  try {
    append(buffer, bytes);
    return ExpectVoid();
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

void SmartBuffer::removeFront(std::size_t bytes)
{
  // todo switch to memory buffer if possible
  _buffer->removeFront(bytes);
}

IBuffer::ExpectVoid SmartBuffer::removeFront(std::size_t bytes,
                                             std::nothrow_t /*unused*/)
{
  try {
    removeFront(bytes);
    return ExpectVoid();
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

std::string SmartBuffer::consumeFront(std::size_t bytes)
{
  // todo switch to memory buffer if possible
  return _buffer->consumeFront(bytes);
}

IBuffer::ExpectStr SmartBuffer::consumeFront(std::size_t bytes,
                                             std::nothrow_t /*unused*/)
{
  try {
    return consumeFront(bytes);
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

SmartBuffer::RawBytes SmartBuffer::consumeAll()
{
  // todo switch to memory buffer
  return _buffer->consumeAll();
}

IBuffer::ExpectRaw SmartBuffer::consumeAll(std::nothrow_t /*unused*/)
{
  try {
    return consumeAll();
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

std::string SmartBuffer::getStr(std::size_t start, std::size_t bytes)
{
  return _buffer->getStr(start, bytes);
}

IBuffer::ExpectStr SmartBuffer::getStr(std::size_t start,
                                       std::size_t bytes,
                                       std::nothrow_t /*unused*/)
{
  try {
    return getStr(start, bytes);
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

SmartBuffer::RawBytes SmartBuffer::getRawBytes(std::size_t start,
                                               std::size_t bytes)
{
  return _buffer->getRawBytes(start, bytes);
}

IBuffer::ExpectRaw SmartBuffer::getRawBytes(std::size_t start,
                                            std::size_t bytes,
                                            std::nothrow_t /*unused*/)
{
  try {
    return getRawBytes(start, bytes);
  } catch (const BufferException& e) {
    return ft::unexpected<BufferException>(e);
  }
}

void SmartBuffer::replace(RawBytes& rawData)
{
  // todo maybe check size first
  _buffer->replace(rawData);
}

IBuffer::ExpectVoid SmartBuffer::replace(RawBytes& rawData,
                                         std::nothrow_t /*unused*/)
{
  try {
    replace(rawData);
    return ExpectVoid();
  } catch (const BufferException& e) {
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

bool SmartBuffer::_fileNeeded(long newBytes)
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
