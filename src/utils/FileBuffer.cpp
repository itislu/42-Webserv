#include "FileBuffer.hpp"

#include <libftpp/expected.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <utils/IBuffer.hpp>

#include <algorithm>
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
  }
  if (!_fileName.empty()) {
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

  _fs.write(data.c_str(), static_cast<std::streamsize>(data.size()));
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errWrite);
  }

  _size += data.size();
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::append(const FileBuffer::Container& buffer,
                                       long bytes)
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

  _fs.write(buffer.data(), bytes);
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errWrite);
  }

  _size += bytes;
  return ExpectVoid();
}

IBuffer::ExpectVoid FileBuffer::removeFront(std::size_t bytes)
{
  bytes = std::min(bytes, _size);

  _fs.seekg(static_cast<std::streamoff>(bytes));
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
  }

  // read/write rest into new tempFile
  const ExpectVoid resB = _saveRemainder();
  if (!resB.has_value()) {
    return resB;
  }

  _size -= bytes;
  return ExpectVoid();
}

IBuffer::ExpectStr FileBuffer::consumeFront(std::size_t bytes)
{
  bytes = std::min(bytes, _size);

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

IBuffer::ExpectStr FileBuffer::_getFront(std::size_t bytes)
{
  // Go to start of file
  _fs.seekg(0, std::ios::beg);
  if (_fs.fail()) {
    return ft::unexpected<BufferException>(errSeek);
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
    if (bufFrom.fail() && !bufFrom.eof()) {
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
