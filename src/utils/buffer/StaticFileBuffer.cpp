#include "StaticFileBuffer.hpp"
#include "libftpp/optional.hpp"
#include "utils/fileUtils.hpp"

#include <cstddef>
#include <libftpp/expected.hpp>
#include <utils/buffer/IBuffer.hpp>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <new>
#include <string>

/* ************************************************************************** */
// INIT

const char* const StaticFileBuffer::errOpen = "FbException: open failed";
const char* const StaticFileBuffer::errSeek = "FbException: seek failed";
const char* const StaticFileBuffer::errFileEmpty = "FbException: file is empty";
const char* const StaticFileBuffer::errOutOfRange =
  "FbException: tried to access out of range";
const char* const StaticFileBuffer::errRead = "FbException: read failed";
const char* const StaticFileBuffer::errWrite = "FbException: write failed";
const char* const StaticFileBuffer::errTell =
  "FbException: tell position failed";

/* ************************************************************************** */
// PUBLIC

StaticFileBuffer::StaticFileBuffer()
  : _size(0)
  , _consumendFront(0)
{
}

StaticFileBuffer::StaticFileBuffer(const std::string& filepath)
  : _fileName(filepath)
  , _size(0)
  , _consumendFront(0)
{
  _fs.open(filepath.c_str(), std::ios::in | std::ios::out | std::ios::binary);
  if (!_fs.is_open()) {
    throw BufferException(errOpen);
  }
  const ft::optional<std::size_t> optSize = getFileSize(filepath);
  if (!optSize.has_value()) {
    throw BufferException(errOpen);
  }
  _size = *optSize;
  seek(0);
}

StaticFileBuffer::~StaticFileBuffer()
{
  if (_fs.is_open()) {
    _fs.close();
  }
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
  pos += _consumendFront;
  if (pos > _size) {
    if (_size == 0) {
      throw BufferException(errFileEmpty);
    }
    throw BufferException(errOutOfRange);
  }
  if (_size == 0) {
    return;
  }

  _fs.seekg(static_cast<std::streamoff>(pos));
  if (_fs.fail()) {
    throw BufferException(errSeek);
  }
}

std::size_t StaticFileBuffer::pos()
{
  const std::streampos pos = _fs.tellg();
  if (pos < std::streampos(0)) {
    throw BufferException(errTell);
  }
  return static_cast<std::size_t>(pos);
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
  return _consumeFront<RawBytes>(_size);
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

// Non-throwing versions

IBuffer::ExpectChr StaticFileBuffer::get(std::nothrow_t /*unused*/)
{
  try {
    return get();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectChr StaticFileBuffer::peek(std::nothrow_t /*unused*/)
{
  try {
    return peek();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid StaticFileBuffer::seek(std::size_t pos,
                                           std::nothrow_t /*unused*/)
{
  try {
    seek(pos);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectPos StaticFileBuffer::pos(std::nothrow_t /*unused*/)
{
  try {
    return pos();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectStr StaticFileBuffer::consumeFront(std::size_t bytes,
                                                  std::nothrow_t /*unused*/)
{
  try {
    return consumeFront(bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw StaticFileBuffer::consumeRawFront(std::size_t bytes,
                                                     std::nothrow_t /*unused*/)
{
  try {
    return consumeRawFront(bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw StaticFileBuffer::consumeAll(std::nothrow_t /*unused*/)
{
  try {
    return consumeAll();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectStr StaticFileBuffer::getStr(std::size_t start,
                                            std::size_t bytes,
                                            std::nothrow_t /*unused*/)
{
  try {
    return getStr(start, bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw StaticFileBuffer::getRawBytes(std::size_t start,
                                                 std::size_t bytes,
                                                 std::nothrow_t /*unused*/)
{
  try {
    return getRawBytes(start, bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

bool StaticFileBuffer::isEmpty() const
{
  return size() == 0;
}

std::size_t StaticFileBuffer::size() const
{
  return _size - _consumendFront;
}

/* ************************************************************************** */
// PRIVATE

char StaticFileBuffer::_getChr(std::fstream::int_type (std::fstream::*func)())
{
  if (_size == 0) {
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
  const ContigContainer front =
    _getData<ContigContainer>(_consumendFront, bytes);
  assert(front.size() == bytes); // Unexpected EOF should not happen
  _consumendFront += front.size();
  return front;
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ContigContainer StaticFileBuffer::_getData(std::size_t start, std::size_t bytes)
{
  if (start >= _size || bytes > _size - start) {
    if (_size == 0) {
      throw BufferException(errFileEmpty);
    }
    throw BufferException(errOutOfRange);
  }
  if (bytes == 0) {
    return ContigContainer();
  }

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
