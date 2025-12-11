#include "SmartBuffer.hpp"

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/FileBuffer.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/MemoryBuffer.hpp>

#include <cstddef>
#include <string>
#include <sys/types.h>

/* ************************************************************************** */
// INIT

const char* const SmartBuffer::errAllocBuffer =
  "SmartBuffer exception: failed to initialize buffer";

const std::size_t SmartBuffer::_defaultMemoryToFileThreshold;
const std::size_t SmartBuffer::_defaultFileToMemoryThreshold;
std::size_t SmartBuffer::_memoryToFileThreshold =
  SmartBuffer::_defaultMemoryToFileThreshold;
std::size_t SmartBuffer::_fileToMemoryThreshold =
  SmartBuffer::_defaultFileToMemoryThreshold;

/* ************************************************************************** */
// PUBLIC

SmartBuffer::SmartBuffer()
  : _usesFile(false)
{
  ft::unique_ptr<MemoryBuffer> newBuffer = ft::make_unique<MemoryBuffer>();
  _buffer = ft::move(newBuffer);
}

std::size_t SmartBuffer::getMemoryToFileThreshold()
{
  return _memoryToFileThreshold;
}

std::size_t SmartBuffer::getFileToMemoryThreshold()
{
  return _fileToMemoryThreshold;
}

void SmartBuffer::setMemoryToFileThreshold(std::size_t value)
{
  _memoryToFileThreshold = value;
}

void SmartBuffer::setFileToMemoryThreshold(std::size_t value)
{
  _fileToMemoryThreshold = value;
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
  _buffer->removeFront(bytes);
  if (_usesFile && _memoryBufferPossible()) {
    _switchToMemoryBuffer();
  }
}

std::string SmartBuffer::consumeFront(std::size_t bytes)
{
  const std::string strBytes = _buffer->consumeFront(bytes);
  if (_usesFile && _memoryBufferPossible()) {
    _switchToMemoryBuffer();
  }
  return strBytes;
}

SmartBuffer::RawBytes SmartBuffer::consumeRawFront(std::size_t bytes)
{
  const IBuffer::RawBytes rawbytes = _buffer->consumeRawFront(bytes);
  if (_usesFile && _memoryBufferPossible()) {
    _switchToMemoryBuffer();
  }
  return rawbytes;
}

SmartBuffer::RawBytes SmartBuffer::consumeAll()
{
  const IBuffer::RawBytes rawbytes = _buffer->consumeAll();
  _switchToMemoryBuffer();
  return rawbytes;
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

void SmartBuffer::reset()
{
  _buffer->reset();
  if (_usesFile) {
    _switchToMemoryBuffer();
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

ssize_t SmartBuffer::send(int fdes, std::size_t bytes)
{
  return _buffer->send(fdes, bytes);
}

bool SmartBuffer::usesFile() const
{
  return _usesFile;
}

const IInBuffer* SmartBuffer::getRawBuffer() const
{
  return _buffer.get();
}

/* ************************************************************************** */
// PRIVATE

bool SmartBuffer::_fileNeeded(std::size_t newBytes)
{
  return _buffer->size() + newBytes >= _memoryToFileThreshold;
}

bool SmartBuffer::_memoryBufferPossible()
{
  return _buffer->size() <= _fileToMemoryThreshold;
}

void SmartBuffer::_switchToFileBuffer()
{
  ft::unique_ptr<FileBuffer> newBuffer = ft::make_unique<FileBuffer>();
  if (!_buffer->isEmpty()) {
    RawBytes data = _buffer->consumeAll();
    newBuffer->replace(data);
  }
  _buffer = ft::move(newBuffer);
  _usesFile = true;
}

void SmartBuffer::_switchToMemoryBuffer()
{
  ft::unique_ptr<MemoryBuffer> newBuffer = ft::make_unique<MemoryBuffer>();
  if (!_buffer->isEmpty()) {
    RawBytes data = _buffer->consumeAll();
    newBuffer->replace(data);
  }
  _buffer = ft::move(newBuffer);
  _usesFile = false;
}
