#include "SmartBuffer.hpp"

// #include <libftpp/expected.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/FileBuffer.hpp>
#include <utils/buffer/IBuffer.hpp>

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
  : _usesFile(true)
{
  ft::unique_ptr<FileBuffer> newBuffer = ft::make_unique<FileBuffer>();
  _buffer = ft::move(newBuffer);
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

std::size_t SmartBuffer::pos()
{
  return _buffer->pos();
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

SmartBuffer::RawBytes SmartBuffer::consumeRawFront(std::size_t bytes)
{
  // todo switch to memory buffer if possible
  return _buffer->consumeRawFront(bytes);
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

void SmartBuffer::moveBufferToFile(const std::string& filepath)
{
  // todo maybe check size first
  _buffer->moveBufferToFile(filepath);
}

bool SmartBuffer::isEmpty() const
{
  return _buffer->isEmpty();
}

std::size_t SmartBuffer::size() const
{
  return _buffer->size();
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
