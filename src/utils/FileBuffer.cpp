#include "FileBuffer.hpp"

#include <libftpp/expected.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <utils/IBuffer.hpp>

#include <cassert>
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
  if (_fs.is_open()) {
    _fs.close();
  }
  if (!_fileName.empty()) {
    (void)std::remove(_fileName.c_str());
  }
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
    return ft::unexpected<BufferException>(errOutOfRange);
  }
  if (_size == 0) {
    return ft::unexpected<BufferException>(errFileEmpty);
  }

  _fs.seekg(static_cast<std::streamoff>(pos));
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
  }
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::append(const std::string& data)
{
  return _append(data.data(), static_cast<std::streamsize>(data.size()));
}

IBuffer::ExpectVoid FileBuffer::append(const FileBuffer::Container& buffer,
                                       long bytes)
{
  return _append(buffer.data(), static_cast<std::streamsize>(bytes));
}

IBuffer::ExpectVoid FileBuffer::removeFront(std::size_t bytes)
{
  ExpectVoid res = seek(bytes);
  if (!res.has_value()) {
    return res;
  }

  // read/write rest into new tempFile
  res = _saveRemainder();
  if (!res.has_value()) {
    return res;
  }

  _size -= bytes;
  return ExpectVoid();
}

IBuffer::ExpectStr FileBuffer::consumeFront(std::size_t bytes)
{
  // go to start of file
  ExpectVoid res = seek(0);
  if (!res.has_value()) {
    return ft::unexpected<BufferException>(res.error());
  }

  // read bytes from the beginning
  const ExpectStr front = _getStr(bytes);
  if (!front.has_value()) {
    return front;
  }
  assert(front->size() == bytes); // Unexpected EOF should not happen

  // read/write rest into new tempFile
  res = _saveRemainder();
  if (!res.has_value()) {
    return ft::unexpected<BufferException>(res.error());
  }

  _size -= bytes;
  return front;
}

std::size_t FileBuffer::size() const
{
  return _size;
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
  filename.append("tmpfile_");
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
        _size = 0;
        return ExpectVoid();
      }
    } else {
      _fs.close();
    }
  }
  // tried to open 100 random files
  return ft::unexpected<BufferException>(errOpen);
}

IBuffer::ExpectChr FileBuffer::_getChr(
  std::fstream::int_type (std::fstream::*func)())
{
  if (_size == 0) {
    return ft::unexpected<BufferException>(errFileEmpty);
  }

  const int chr = (_fs.*func)();
  if (_fs.fail()) {
    if (_fs.eof()) {
      return ft::unexpected<BufferException>(errOutOfRange);
    }
    return ft::unexpected<BufferException>(errRead);
  }
  return chr;
}

IBuffer::ExpectStr FileBuffer::_getStr(std::size_t bytes)
{
  if (bytes == 0) {
    return std::string();
  }

  std::string front(bytes, '\0');
  _fs.read(&front[0], static_cast<std::streamsize>(bytes));
  if (_fs.bad()) {
    return ft::unexpected<BufferException>(errRead);
  }
  const std::streamsize actuallyRead = _fs.gcount();
  front.resize(static_cast<std::size_t>(actuallyRead));
  return front;
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
    return ft::unexpected<BufferException>(errSeek);
  }

  _fs.write(data, bytes);
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errWrite);
  }

  _size += bytes;
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::_saveRemainder()
{
  // Create a temporary file for the remainder
  FileBuffer tmp;
  ExpectVoid res = tmp._openTmpFile();
  if (!res.has_value()) {
    return res;
  }

  // Copy the remaining data
  res = _copyData(_fs, tmp._fs);
  if (!res.has_value()) {
    return res;
  }

  res = _replaceCurrFile(tmp);
  if (!res.has_value()) {
    return res;
  }
  return ExpectVoid();
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
IBuffer::ExpectVoid FileBuffer::_copyData(std::fstream& bufFrom,
                                          std::fstream& bufTo)
{
  char buffer[_copyBufferSize];
  while (!bufFrom.eof()) {
    bufFrom.read(buffer, _copyBufferSize);
    if (bufFrom.bad()) {
      return ft::unexpected<BufferException>(errRead);
    }
    const std::streamsize bytesRead = bufFrom.gcount();
    if (bytesRead > 0) {
      bufTo.write(buffer, bytesRead);
      if (bufTo.fail()) {
        return ft::unexpected<BufferException>(errWrite);
      }
    }
  }
  return ExpectVoid();
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

IBuffer::ExpectVoid FileBuffer::_replaceCurrFile(FileBuffer& tmpFb)
{
  // close and remove curr file
  _fs.close();
  (void)std::remove(_fileName.c_str());

  // rename curr file
  _fileName = tmpFb._fileName;
  _fs.open(_fileName.c_str(), std::ios::in | std::ios::out | std::ios::binary);
  if (!_fs.is_open()) {
    _fileName.clear();
    return ft::unexpected<BufferException>(errOpen);
  }

  // remove filename of tmp file so it wont be removed in destructor
  tmpFb._fileName.clear();
  return ExpectVoid();
}
