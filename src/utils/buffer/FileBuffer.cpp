#include "FileBuffer.hpp"

#include <utils/buffer/IBuffer.hpp>
#include <utils/fileUtils.hpp>

#include <algorithm>
#include <cassert>
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

const char* const FileBuffer::errOpen = "FileBuffer exception: open failed";
const char* const FileBuffer::errSeek = "FileBuffer exception: seek failed";
const char* const FileBuffer::errFileEmpty =
  "FileBuffer exception: file is empty";
const char* const FileBuffer::errOutOfRange =
  "FileBuffer exception: tried to access out of range";
const char* const FileBuffer::errRead = "FileBuffer exception: read failed";
const char* const FileBuffer::errWrite = "FileBuffer exception: write failed";
const char* const FileBuffer::errTell =
  "FileBuffer exception: tell position failed";
const char* const FileBuffer::errRename =
  "FileBuffer exception: rename file failed";

/* ************************************************************************** */
// PUBLIC

FileBuffer::FileBuffer()
  : _size(0)
{
}

FileBuffer::~FileBuffer()
{
  _removeCurrFile();
}

// Throwing versions

char FileBuffer::get()
{
  return _getChr(&std::fstream::get);
}

char FileBuffer::peek()
{
  return _getChr(&std::fstream::peek);
}

/**
 * Seeking beyond EOF is an error.
 * Seeking to the beginning of an empty buffer is therefore allowed.
 */
void FileBuffer::seek(std::size_t pos)
{
  if (pos > _size) {
    if (_size == 0) {
      throw BufferException(errFileEmpty);
    }
    throw BufferException(errOutOfRange);
  }
  // pos is 0; size == 0 means no file open.
  if (_size == 0) {
    return;
  }

  _fs.seekg(static_cast<std::streamoff>(pos));
  if (_fs.fail()) {
    throw BufferException(errSeek);
  }
}

std::size_t FileBuffer::pos()
{
  const std::streampos pos = _fs.tellg();
  if (pos == std::streampos(-1)) {
    throw BufferException(errTell);
  }
  return static_cast<std::size_t>(pos);
}

void FileBuffer::append(const std::string& data)
{
  _append(data.data(), static_cast<std::streamsize>(data.size()));
}

void FileBuffer::append(const FileBuffer::RawBytes& buffer, std::size_t bytes)
{
  _append(buffer.data(), static_cast<std::streamsize>(bytes));
}

void FileBuffer::removeFront(std::size_t bytes)
{
  if (_size == 0) {
    throw BufferException(errFileEmpty);
  }

  seek(bytes);
  // read/write rest into new tempFile
  _saveRemainder();
}

std::string FileBuffer::consumeFront(std::size_t bytes)
{
  return _consumeFront<std::string>(bytes);
}

FileBuffer::RawBytes FileBuffer::consumeRawFront(std::size_t bytes)
{
  return _consumeFront<RawBytes>(bytes);
}

FileBuffer::RawBytes FileBuffer::consumeAll()
{
  return _consumeFront<RawBytes>(_size);
}

std::string FileBuffer::getStr(std::size_t start, std::size_t bytes)
{
  return _getData<std::string>(start, bytes);
}

FileBuffer::RawBytes FileBuffer::getRawBytes(std::size_t start,
                                             std::size_t bytes)
{
  return _getData<RawBytes>(start, bytes);
}

void FileBuffer::replace(RawBytes& rawData)
{
  _removeCurrFile();
  append(rawData, rawData.size());
}

void FileBuffer::moveBufferToFile(const std::string& filepath)
{
  if (_size == 0) {
    throw BufferException(errFileEmpty);
  }
  if (_fs.is_open()) {
    _fs.close();
  }

  const int res = std::rename(_fileName.c_str(), filepath.c_str());
  if (res != 0) {
    throw BufferException(errRename);
  }

  _fileName.clear();
  _size = 0;
}

bool FileBuffer::isEmpty() const
{
  return _size == 0;
}

std::size_t FileBuffer::size() const
{
  return _size;
}

ssize_t FileBuffer::send(int fdes, std::size_t bytes)
{
  bytes = std::min(bytes, _size);
  IBuffer::RawBytes buff = _getData<RawBytes>(0, bytes);
  const ssize_t bytesSent = ::send(fdes, buff.data(), buff.size(), 0);
  if (bytesSent > 0) {
    removeFront(bytesSent);
  }
  return bytesSent;
}

/* ************************************************************************** */
// PRIVATE

void FileBuffer::_openTmpFile()
{
  assert(!_fs.is_open());
  const std::size_t countMax = 100;
  std::size_t count = 0;
  for (; count < countMax; ++count) {
    _fileName = getRandomFileName(".tmp");
    _fs.open(_fileName.c_str(), std::ios::in);
    // check if file does not exist
    if (!_fs.is_open()) {
      // create file
      _fs.open(_fileName.c_str(),
               std::ios::in | std::ios::out | std::ios::binary |
                 std::ios::trunc);
      if (_fs.is_open()) {
        return;
      }
    } else {
      _fs.close();
    }
  }
  // tried to open 100 random files
  throw BufferException(errOpen);
}

char FileBuffer::_getChr(std::fstream::int_type (std::fstream::*func)())
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
ContigContainer FileBuffer::_consumeFront(std::size_t bytes)
{
  // read bytes from the beginning
  const ContigContainer front = _getData<ContigContainer>(0, bytes);
  assert(front.size() == bytes); // Unexpected EOF should not happen

  // read/write rest into new tempFile
  _saveRemainder();

  return front;
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ContigContainer FileBuffer::_getData(std::size_t start, std::size_t bytes)
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

void FileBuffer::_append(const char* data, std::streamsize bytes)
{
  if (!_fs.is_open()) {
    _openTmpFile();
  }

  // Go to end and add new data
  _fs.seekp(0, std::ios::end);
  if (_fs.fail()) {
    throw BufferException(errSeek);
  }

  _fs.write(data, bytes);
  if (_fs.fail()) {
    throw BufferException(errWrite);
  }

  _size += bytes;
}

void FileBuffer::_saveRemainder()
{
  if (_fs.eof() || _fs.peek() == EOF) {
    // No remainder
    _removeCurrFile();
    return;
  }

  // Create a temporary file for the remainder
  FileBuffer tmp;
  tmp._openTmpFile();

  // Copy the remaining data
  tmp._copyFrom(*this);

  _replaceCurrFile(tmp);
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
void FileBuffer::_copyFrom(FileBuffer& src)
{
  std::fstream& fsSrc = src._fs;
  char buffer[_copyBufferSize];
  while (!fsSrc.eof()) {
    fsSrc.read(buffer, _copyBufferSize);
    if (fsSrc.bad()) {
      throw BufferException(errRead);
    }
    const std::streamsize bytesRead = fsSrc.gcount();
    if (bytesRead > 0) {
      _fs.write(buffer, bytesRead);
      if (_fs.fail()) {
        throw BufferException(errWrite);
      }
      _size += static_cast<std::size_t>(bytesRead);
    }
  }
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

/**
 * Close curr file and try to open tmp file.
 * If fail, reopen curr file and keep it; tmpFb will remove its tmp file.
 * If success, swap filename and destructor of tmpFb will remove old curr file.
 */
void FileBuffer::_replaceCurrFile(FileBuffer& tmpFb)
{
  if (_fs.is_open()) {
    _fs.close();
  }
  _fs.open(tmpFb._fileName.c_str(),
           std::ios::in | std::ios::out | std::ios::binary);
  if (!_fs.is_open()) {
    // Fail.
    _fs.open(_fileName.c_str(),
             std::ios::in | std::ios::out | std::ios::binary);
    throw BufferException(errOpen);
  }

  // Success.
  using std::swap;
  swap(_fileName, tmpFb._fileName);
  swap(_size, tmpFb._size);
}

void FileBuffer::_removeCurrFile()
{
  if (_fileName.empty()) {
    return;
  }
  if (_fs.is_open()) {
    _fs.close();
  }
  (void)std::remove(_fileName.c_str());
  _fileName.clear();
  _size = 0;
}
