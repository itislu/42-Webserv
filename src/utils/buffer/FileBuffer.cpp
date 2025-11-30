#include "FileBuffer.hpp"

#include <libftpp/expected.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/fileUtils.hpp>

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

const char* const FileBuffer::errOpen = "FbException: open failed";
const char* const FileBuffer::errSeek = "FbException: seek failed";
const char* const FileBuffer::errFileEmpty = "FbException: file is empty";
const char* const FileBuffer::errOutOfRange =
  "FbException: tried to access out of range";
const char* const FileBuffer::errRead = "FbException: read failed";
const char* const FileBuffer::errWrite = "FbException: write failed";
const char* const FileBuffer::errTell = "FbException: tell position failed";
const char* const FileBuffer::errRename = "FbException: rename file failed";

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
  if (pos < std::streampos(0)) {
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
  if (_fileName.empty()) {
    throw BufferException(errRename);
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

// Non-throwing versions

IBuffer::ExpectChr FileBuffer::get(std::nothrow_t /*unused*/)
{
  try {
    return get();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectChr FileBuffer::peek(std::nothrow_t /*unused*/)
{
  try {
    return peek();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid FileBuffer::seek(std::size_t pos, std::nothrow_t /*unused*/)
{
  try {
    seek(pos);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectPos FileBuffer::pos(std::nothrow_t /*unused*/)
{
  try {
    return pos();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid FileBuffer::append(const std::string& data,
                                       std::nothrow_t /*unused*/)
{
  try {
    append(data);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid FileBuffer::append(const FileBuffer::RawBytes& buffer,
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

IBuffer::ExpectVoid FileBuffer::removeFront(std::size_t bytes,
                                            std::nothrow_t /*unused*/)
{
  try {
    removeFront(bytes);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectStr FileBuffer::consumeFront(std::size_t bytes,
                                            std::nothrow_t /*unused*/)
{
  try {
    return consumeFront(bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw FileBuffer::consumeRawFront(std::size_t bytes,
                                               std::nothrow_t /*unused*/)
{
  try {
    return consumeRawFront(bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw FileBuffer::consumeAll(std::nothrow_t /*unused*/)
{
  try {
    return consumeAll();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectStr FileBuffer::getStr(std::size_t start,
                                      std::size_t bytes,
                                      std::nothrow_t /*unused*/)
{
  try {
    return getStr(start, bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectRaw FileBuffer::getRawBytes(std::size_t start,
                                           std::size_t bytes,
                                           std::nothrow_t /*unused*/)
{
  try {
    return getRawBytes(start, bytes);
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid FileBuffer::replace(RawBytes& rawData,
                                        std::nothrow_t /*unused*/)
{
  try {
    replace(rawData);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid FileBuffer::moveBufferToFile(const std::string& filepath,
                                                 std::nothrow_t /*unused*/)
{
  try {
    moveBufferToFile(filepath);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

bool FileBuffer::isEmpty() const
{
  return _size == 0;
}

std::size_t FileBuffer::size() const
{
  return _size;
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
 * If success, swap filename and destructor of tmpFb will remove old curr file.
 * If fail, reopen curr file and keep it; tmpFb will remove its tmp file.
 */
void FileBuffer::_replaceCurrFile(FileBuffer& tmpFb)
{
  _fs.close();
  _fs.open(tmpFb._fileName.c_str(),
           std::ios::in | std::ios::out | std::ios::binary);
  if (!_fs.is_open()) {
    _fs.open(_fileName.c_str(),
             std::ios::in | std::ios::out | std::ios::binary);
    throw BufferException(errOpen);
  }
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
