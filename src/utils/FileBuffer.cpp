#include "FileBuffer.hpp"

#include <cstdlib>
#include <ctime>
#include <libftpp/expected.hpp>
#include <libftpp/string.hpp>
#include <utils/IBuffer.hpp>

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <sys/stat.h>

/* ************************************************************************** */
// INIT

const char* const FileBuffer::errOpen = "FbException: open failed";
const char* const FileBuffer::errSeek = "FbException: seek failed";
const char* const FileBuffer::errFileEmpty = "FbException: file is empty";
const char* const FileBuffer::errFileNotOpened =
  "FbException: file is not opened";
const char* const FileBuffer::errEof = "FbException: tried to get end of file";
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
    (void)std::remove(_fileName.c_str());
  }
}

IBuffer::ExpectChr FileBuffer::get()
{
  if (!_fs.is_open()) {
    return ft::unexpected<BufferException>(errFileNotOpened);
  }

  const int chr = _fs.get();
  if (_fs.fail()) {
    if (_fs.eof()) {
      return ft::unexpected<BufferException>(errEof);
    }
    return ft::unexpected<BufferException>(errRead);
  }
  return chr;
}

IBuffer::ExpectChr FileBuffer::peek()
{
  if (!_fs.is_open()) {
    return ft::unexpected<BufferException>(errFileNotOpened);
  }

  const int chr = _fs.peek();
  if (_fs.fail()) {
    if (_fs.eof()) {
      return ft::unexpected<BufferException>(errEof);
    }
    return ft::unexpected<BufferException>(errRead);
  }
  return chr;
}

IBuffer::ExpectVoid FileBuffer::seek(std::size_t pos)
{
  if (!_fs.is_open()) {
    return ft::unexpected<BufferException>(errFileNotOpened);
  }

  _fs.seekg(static_cast<std::streamoff>(pos));
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
  }
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::append(const std::string& data)
{
  if (!_fs.is_open()) {
    return ft::unexpected<BufferException>(errFileNotOpened);
  }

  // Go to end and add new data
  _fs.seekp(0, std::ios::end);
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
  }

  _fs << data;
  _calcSize();
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::append(const FileBuffer::Container& buffer,
                                       long bytes)
{
  if (!_fs.is_open()) {
    return ft::unexpected<BufferException>(errFileNotOpened);
  }

  // Go to end and add new data
  _fs.seekp(0, std::ios::end);
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
  }

  _fs.write(buffer.data(), bytes);
  if (_fs.fail() || _fs.bad()) {
    return ft::unexpected<BufferException>(errWrite);
  }

  _calcSize();
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::removeFront(std::size_t bytes)
{
  _fs.seekg(static_cast<std::streamoff>(bytes));
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
  }

  // read/write rest into new tempFile
  const ExpectVoid resB = _saveRemainder();
  if (!resB.has_value()) {
    return resB;
  }
  _calcSize();
  return ExpectVoid();
}

IBuffer::ExpectStr FileBuffer::consumFront(std::size_t bytes)
{
  // Go start of file
  _fs.seekg(0);
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
  }

  // read bytes from the beginning
  const ExpectStr front = _getFront(bytes);
  if (!front.has_value()) {
    return front;
  }

  // read/write rest into new tempFile
  const ExpectVoid resB = _saveRemainder();
  if (!resB.has_value()) {
    return ft::unexpected<BufferException>(resB.error().what());
  }
  _calcSize();
  return front;
}

std::size_t FileBuffer::size() const
{
  return _size;
}

// NOLINTBEGIN(cert-msc30-c, cert-msc30-cpp, cert-msc32-c, cert-msc50-cpp,
// cert-msc51-cpp)
std::string FileBuffer::_getRandomeFileName()
{
  static bool seeded = false;

  if (!seeded) {
    seeded = true;
    std::srand(std::time(0));
  }

  std::string filename;
  filename.append("tmpfile_");
  filename.append(ft::to_string(std::rand()));
  filename.append(ft::to_string(std::rand()));
  return filename;
}
// NOLINTEND(cert-msc30-c, cert-msc30-cpp, cert-msc32-c, cert-msc50-cpp,
// cert-msc51-cpp)

IBuffer::ExpectVoid FileBuffer::openTmpFile()
{
  const std::size_t countMax = 100;
  std::size_t count = 0;
  while (count < countMax) {
    _fileName = _getRandomeFileName();
    _fs.open(_fileName.c_str(), std::ios::in);
    // check if file does not exit
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
    ++count;
  }
  // tried to open 100 randome files
  return ft::unexpected<BufferException>(errOpen);
}

/* ************************************************************************** */
// PRIVATE

IBuffer::ExpectStr FileBuffer::_getFront(std::size_t bytes)
{
  if (!_fs.is_open()) {
    return ft::unexpected<BufferException>(errFileNotOpened);
  }

  std::string front(bytes, '\0');
  _fs.read(&front[0], static_cast<std::streamsize>(bytes));
  const std::streamsize actuallyRead = _fs.gcount();
  if (actuallyRead == 0) {
    return ft::unexpected<BufferException>(errFileEmpty);
  }
  front.resize(static_cast<std::size_t>(actuallyRead));
  return front;
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
IBuffer::ExpectVoid FileBuffer::_saveRemainder()
{
  // Create a temporary file for the remainder
  FileBuffer tmp;
  const ExpectVoid resA = tmp.openTmpFile();
  if (!resA.has_value()) {
    return resA;
  }

  // Copy the remaining data
  char buffer[_copyBufferSize];
  while (_fs.good()) {
    _fs.read(buffer, _copyBufferSize);
    const std::streamsize bytesRead = _fs.gcount();
    if (bytesRead > 0) {
      tmp._fs.write(buffer, bytesRead);
      if (tmp._fs.fail() || tmp._fs.bad()) {
        return ft::unexpected<BufferException>(errWrite);
      }
    }
  }
  tmp._fs.close();

  // Replace current file with tmp file
  _fs.close();
  (void)std::remove(_fileName.c_str());
  _fileName = tmp._fileName;
  _fs.open(_fileName.c_str(), std::ios::in | std::ios::out | std::ios::binary);
  if (!_fs.is_open()) {
    return ft::unexpected<BufferException>(errOpen);
  }
  return ExpectVoid();
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

IBuffer::ExpectVoid FileBuffer::_calcSize()
{
  _fs.seekg(0);
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
  }

  struct stat info = {};
  stat(_fileName.c_str(), &info);
  _size = info.st_size;
  return ExpectVoid();
}
