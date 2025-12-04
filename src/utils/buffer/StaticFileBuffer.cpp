#include "StaticFileBuffer.hpp"

#include <libftpp/optional.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/fileUtils.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

/* ************************************************************************** */
// INIT

const char* const StaticFileBuffer::errOpen =
  "StaticFileBuffer exception: open failed";
const char* const StaticFileBuffer::errSeek =
  "StaticFileBuffer exception: seek failed";
const char* const StaticFileBuffer::errFileEmpty =
  "StaticFileBuffer exception: file is empty";
const char* const StaticFileBuffer::errOutOfRange =
  "StaticFileBuffer exception: tried to access out of range";
const char* const StaticFileBuffer::errRead =
  "StaticFileBuffer exception: read failed";
const char* const StaticFileBuffer::errTell =
  "StaticFileBuffer exception: tell position failed";

/* ************************************************************************** */
// PUBLIC

StaticFileBuffer::StaticFileBuffer(const std::string& filepath)
  : _fileName(filepath)
  , _size(0)
  , _consumedFront(0)
{
  _fs.open(filepath.c_str(), std::ios::in | std::ios::binary);
  if (!_fs.is_open()) {
    throw BufferException(errOpen);
  }
  const ft::optional<std::size_t> optSize = getFileSize(filepath);
  if (!optSize.has_value()) {
    throw BufferException(errOpen);
  }
  _size = *optSize;
}

// Throwing versions

char StaticFileBuffer::get()
{
  return _getChr(&std::fstream::get);
}

char StaticFileBuffer::peek()
{
  return _getChr(&std::fstream::peek);
}

/**
 * Seeking beyond EOF is an error.
 * Seeking to the beginning of an empty buffer is therefore allowed.
 */
void StaticFileBuffer::seek(std::size_t pos)
{
  if (pos > size()) {
    if (size() == 0) {
      throw BufferException(errFileEmpty);
    }
    throw BufferException(errOutOfRange);
  }
  if (size() == 0) {
    return;
  }

  _fs.seekg(static_cast<std::streamoff>(pos + _consumedFront));
  if (_fs.fail()) {
    throw BufferException(errSeek);
  }
}

std::size_t StaticFileBuffer::pos()
{
  const std::streampos pos = _fs.tellg();
  if (pos == std::streampos(-1)) {
    throw BufferException(errTell);
  }
  const std::size_t sizetPos = static_cast<std::size_t>(pos);
  assert(_consumedFront <= sizetPos);
  return sizetPos - _consumedFront;
}

std::string StaticFileBuffer::consumeFront(std::size_t bytes)
{
  return _consumeFront<std::string>(bytes);
}

StaticFileBuffer::RawBytes StaticFileBuffer::consumeRawFront(std::size_t bytes)
{
  return _consumeFront<RawBytes>(bytes);
}

StaticFileBuffer::RawBytes StaticFileBuffer::consumeAll()
{
  return _consumeFront<RawBytes>(size());
}

std::string StaticFileBuffer::getStr(std::size_t start, std::size_t bytes)
{
  return _getData<std::string>(start, bytes);
}

StaticFileBuffer::RawBytes StaticFileBuffer::getRawBytes(std::size_t start,
                                                         std::size_t bytes)
{
  return _getData<RawBytes>(start, bytes);
}

bool StaticFileBuffer::isEmpty() const
{
  return size() == 0;
}

std::size_t StaticFileBuffer::size() const
{
  return _size - _consumedFront;
}

ssize_t StaticFileBuffer::send(int fdes, std::size_t bytes)
{
  bytes = std::min(bytes, size());
  IBuffer::RawBytes buff = _getData<RawBytes>(0, bytes);
  const ssize_t bytesSent = ::send(fdes, buff.data(), buff.size(), 0);

  if (bytesSent > 0) {
    _consumedFront += bytesSent;
  }
  return bytesSent;
}

/* ************************************************************************** */
// PRIVATE

char StaticFileBuffer::_getChr(std::fstream::int_type (std::ifstream::*func)())
{
  if (size() == 0) {
    throw BufferException(errFileEmpty);
  }

  const int chr = (_fs.*func)();
  if (_fs.fail()) {
    if (_fs.eof()) {
      throw BufferException(errOutOfRange);
    }
    throw BufferException(errRead);
  }
  return static_cast<char>(chr);
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ContigContainer StaticFileBuffer::_consumeFront(std::size_t bytes)
{
  // read bytes from the beginning
  const ContigContainer front = _getData<ContigContainer>(0, bytes);
  assert(front.size() == bytes); // Unexpected EOF should not happen
  _consumedFront += front.size();
  return front;
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ContigContainer StaticFileBuffer::_getData(std::size_t start, std::size_t bytes)
{
  if (start >= size() || bytes > size() - start) {
    if (size() == 0) {
      throw BufferException(errFileEmpty);
    }
    throw BufferException(errOutOfRange);
  }
  if (bytes == 0) {
    return ContigContainer();
  }

  seek(start);

  ContigContainer front;
  front.resize(bytes);

  _fs.read(&front[0], static_cast<std::streamsize>(bytes));
  if (_fs.bad()) {
    throw BufferException(errRead);
  }
  const std::streamsize actuallyRead = _fs.gcount();
  front.resize(static_cast<std::size_t>(actuallyRead));
  return front;
}
