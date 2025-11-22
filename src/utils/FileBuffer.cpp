#include "FileBuffer.hpp"
#include "utils/printUtils.hpp"

#include <iostream>
#include <libftpp/expected.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <utils/IBuffer.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <ios>
#include <iosfwd>
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

IBuffer::ExpectChr FileBuffer::get()
{
  return _getChr(&std::fstream::get);
}

IBuffer::ExpectChr FileBuffer::peek()
{
  return _getChr(&std::fstream::peek);
}

IBuffer::ExpectVoid FileBuffer::seek(std::size_t pos)
{
  if (pos > _size) {
    if (_size == 0) {
      return handleUnexpected(errFileEmpty);
    }
    return handleUnexpected(errOutOfRange);
  }
  if (_size == 0) {
    return ExpectVoid();
  }

  _fs.seekg(static_cast<std::streamoff>(pos));
  if (_fs.fail()) {
    return handleUnexpected(errSeek);
  }
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::append(const std::string& data)
{
  return _append(data.data(), static_cast<std::streamsize>(data.size()));
}

IBuffer::ExpectVoid FileBuffer::append(const FileBuffer::RawBytes& buffer,
                                       long bytes)
{
  return _append(buffer.data(), static_cast<std::streamsize>(bytes));
}

IBuffer::ExpectVoid FileBuffer::removeFront(std::size_t bytes)
{
  if (_size == 0) {
    return handleUnexpected(errFileEmpty);
  }

  const ExpectVoid res = seek(bytes);
  if (!res.has_value()) {
    return res;
  }
  // read/write rest into new tempFile
  return _saveRemainder();
}

IBuffer::ExpectStr FileBuffer::consumeFront(std::size_t bytes)
{
  return _consumeFront<std::string>(bytes);
}

IBuffer::ExpectRaw FileBuffer::consumeAll()
{
  return _consumeFront<RawBytes>(_size);
}

IBuffer::ExpectStr FileBuffer::getStr(std::size_t start, std::size_t bytes)
{
  return _getData<std::string>(start, bytes);
}

IBuffer::ExpectRaw FileBuffer::getRawBytes(std::size_t start, std::size_t bytes)
{
  return _getData<RawBytes>(start, bytes);
}

IBuffer::ExpectVoid FileBuffer::replace(RawBytes& rawData)
{
  _removeCurrFile();
  return append(rawData, static_cast<long>(rawData.size()));
}

bool FileBuffer::isEmpty() const
{
  return _size == 0;
}

std::size_t FileBuffer::size() const
{
  return _size;
}

void FileBuffer::print()
{
  const std::streampos oldPos = _fs.tellg();
  const std::ios::iostate oldState = _fs.rdstate();

  char chr = '\0';
  std::cout << "'";
  while (_fs.get(chr).good()) {
    printEscapedChar(chr);
  }
  std::cout << "'\n";
  _fs.clear(oldState); // after get last, fstream is in fail state
  _fs.seekg(oldPos);
}

/* ************************************************************************** */
// PRIVATE

// NOLINTBEGIN(bugprone-random-generator-seed, misc-predictable-rand)
static std::string getRandomFileName()
{
  static bool seeded = false;

  if (!seeded) {
    seeded = true;
    std::srand(std::time(FT_NULLPTR));
  }

  std::string filename;
  filename.append(".tmpfile_");
  filename.append(ft::to_string(std::rand()));
  filename.append(ft::to_string(std::rand()));
  return filename;
}
// NOLINTEND(bugprone-random-generator-seed, misc-predictable-rand)

IBuffer::ExpectVoid FileBuffer::_openTmpFile()
{
  assert(!_fs.is_open());
  const std::size_t countMax = 100;
  std::size_t count = 0;
  for (; count < countMax; ++count) {
    _fileName = getRandomFileName();
    _fs.open(_fileName.c_str(), std::ios::in);
    // check if file does not exist
    if (!_fs.is_open()) {
      // create file
      _fs.open(_fileName.c_str(),
               std::ios::in | std::ios::out | std::ios::binary |
                 std::ios::trunc);
      if (_fs.is_open()) {
        return ExpectVoid();
      }
    } else {
      _fs.close();
    }
  }
  // tried to open 100 random files
  return handleUnexpected(errOpen);
}

IBuffer::ExpectChr FileBuffer::_getChr(
  std::fstream::int_type (std::fstream::*func)())
{
  if (_size == 0) {
    return handleUnexpected(errFileEmpty);
  }

  const int chr = (_fs.*func)();
  if (_fs.fail()) {
    if (_fs.eof()) {
      return handleUnexpected(errOutOfRange);
    }
    return handleUnexpected(errRead);
  }
  return static_cast<char>(chr);
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ft::expected<ContigContainer, FileBuffer::BufferException>
FileBuffer::_consumeFront(std::size_t bytes)
{
  typedef ft::expected<ContigContainer, BufferException> ExpectCont;

  // read bytes from the beginning
  const ExpectCont front = _getData<ContigContainer>(0, bytes);
  if (!front.has_value()) {
    return front;
  }
  assert(front->size() == bytes); // Unexpected EOF should not happen

  // read/write rest into new tempFile
  const ExpectVoid res = _saveRemainder();
  if (!res.has_value()) {
    return handleUnexpected(res.error().what());
  }

  return front;
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ft::expected<ContigContainer, FileBuffer::BufferException> FileBuffer::_getData(
  std::size_t start,
  std::size_t bytes)
{
  typedef ft::expected<ContigContainer, BufferException> ExpectCont;

  if (start >= _size || bytes > _size - start) {
    if (_size == 0) {
      return handleUnexpected(errFileEmpty);
    }
    return handleUnexpected(errOutOfRange);
  }
  if (bytes == 0) {
    return ContigContainer();
  }

  const ExpectVoid res = seek(start);
  if (!res.has_value()) {
    return handleUnexpected(res.error().what());
  }

  ExpectCont cont;
  ContigContainer& front = cont.value();
  front.resize(bytes);

  _fs.read(&front[0], static_cast<std::streamsize>(bytes));
  if (_fs.bad()) {
    return handleUnexpected(errRead);
  }
  const std::streamsize actuallyRead = _fs.gcount();
  front.resize(static_cast<std::size_t>(actuallyRead));

  return cont;
}

IBuffer::ExpectVoid FileBuffer::_append(const char* data, std::streamsize bytes)
{
  if (!_fs.is_open()) {
    const ExpectVoid res = _openTmpFile();
    if (!res.has_value()) {
      return res;
    }
  }

  // Go to end and add new data
  _fs.seekp(0, std::ios::end);
  if (_fs.fail()) {
    return handleUnexpected(errSeek);
  }

  _fs.write(data, bytes);
  if (_fs.fail()) {
    return handleUnexpected(errWrite);
  }

  _size += bytes;
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::_saveRemainder()
{
  if (_fs.eof() || _fs.peek() == EOF) {
    // No remainder
    _removeCurrFile();
    return ExpectVoid();
  }

  // Create a temporary file for the remainder
  FileBuffer tmp;
  ExpectVoid res = tmp._openTmpFile();
  if (!res.has_value()) {
    return res;
  }

  // Copy the remaining data
  res = tmp._copyFrom(*this);
  if (!res.has_value()) {
    return res;
  }

  return _replaceCurrFile(tmp);
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
IBuffer::ExpectVoid FileBuffer::_copyFrom(FileBuffer& src)
{
  std::fstream& fsSrc = src._fs;
  char buffer[_copyBufferSize];
  while (!fsSrc.eof()) {
    fsSrc.read(buffer, _copyBufferSize);
    if (fsSrc.bad()) {
      return handleUnexpected(errRead);
    }
    const std::streamsize bytesRead = fsSrc.gcount();
    if (bytesRead > 0) {
      _fs.write(buffer, bytesRead);
      if (_fs.fail()) {
        return handleUnexpected(errWrite);
      }
      _size += static_cast<std::size_t>(bytesRead);
    }
  }
  return ExpectVoid();
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

/**
 * Close curr file and try to open tmp file.
 * If success, swap filename and destructor of tmpFb will remove old curr file.
 * If fail, reopen curr file and keep it; tmpFb will remove its tmp file.
 */
IBuffer::ExpectVoid FileBuffer::_replaceCurrFile(FileBuffer& tmpFb)
{
  _fs.close();
  _fs.open(tmpFb._fileName.c_str(),
           std::ios::in | std::ios::out | std::ios::binary);
  if (!_fs.is_open()) {
    _fs.open(_fileName.c_str(),
             std::ios::in | std::ios::out | std::ios::binary);
    return handleUnexpected(errOpen);
  }
  using std::swap;
  swap(_fileName, tmpFb._fileName);
  swap(_size, tmpFb._size);

  return ExpectVoid();
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
