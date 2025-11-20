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
    return ft::unexpected<BufferException>(errOutOfRange);
  }
  if (_size == 0) {
    return ExpectVoid();
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

IBuffer::ExpectVoid FileBuffer::append(const FileBuffer::RawBytes& buffer,
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
  if (_size == 0) {
    return ft::unexpected<BufferException>(errFileEmpty);
  }

  // read/write rest into new tempFile
  res = _saveRemainder();
  if (!res.has_value()) {
    return res;
  }

  return ExpectVoid();
}

IBuffer::ExpectStr FileBuffer::consumeFront(std::size_t bytes)
{
  return _consumeFront<std::string>(bytes);
}

IBuffer::ExpectRaw FileBuffer::consumeAll()
{
  ExpectRaw result;
  RawBytes& rawBytes = result.value();
  rawBytes.resize(_size);
  _fs.read(&rawBytes[0], static_cast<std::streamsize>(_size));
  const std::streamsize actuallyRead = _fs.gcount();
  if (actuallyRead == 0) {
    return ft::unexpected<BufferException>(errRead);
  }
  if (_fs.is_open()) {
    _fs.close();
  }
  if (!_fileName.empty()) {
    (void)std::remove(_fileName.c_str());
  }
  return result;
}

IBuffer::ExpectVoid FileBuffer::replace(RawBytes& rawData)
{
  if (_size > 0) {
    if (_fs.is_open()) {
      _fs.close();
    }
    if (!_fileName.empty()) {
      (void)std::remove(_fileName.c_str());
    }
  }
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
  return static_cast<char>(chr);
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ft::expected<ContigContainer, FileBuffer::BufferException>
FileBuffer::_consumeFront(std::size_t bytes)
{
  typedef ft::expected<ContigContainer, BufferException> ExpectCont;

  // go to start of file
  ExpectVoid res = seek(0);
  if (!res.has_value()) {
    return ft::unexpected<BufferException>(res.error());
  }
  if (_size == 0) {
    return ft::unexpected<BufferException>(errFileEmpty);
  }

  // read bytes from the beginning
  const ExpectCont front = _getData<ContigContainer>(bytes);
  if (!front.has_value()) {
    return front;
  }
  assert(front->size() == bytes); // Unexpected EOF should not happen

  // read/write rest into new tempFile
  res = _saveRemainder();
  if (!res.has_value()) {
    return ft::unexpected<BufferException>(res.error());
  }

  return front;
}

// Template can be in source file if only used here.
template<typename ContigContainer>
ft::expected<ContigContainer, FileBuffer::BufferException> FileBuffer::_getData(
  std::size_t bytes)
{
  if (bytes == 0) {
    return ContigContainer();
  }
  if (bytes > _size) {
    return ft::unexpected<BufferException>(errOutOfRange);
  }

  ExpectStr res;
  ContigContainer& front = res.value();
  front.resize(bytes);

  _fs.read(&front[0], static_cast<std::streamsize>(bytes));
  if (_fs.bad()) {
    return ft::unexpected<BufferException>(errRead);
  }
  const std::streamsize actuallyRead = _fs.gcount();
  front.resize(static_cast<std::size_t>(actuallyRead));

  return res;
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

  res = _replaceCurrFile(tmp);
  if (!res.has_value()) {
    return res;
  }
  return ExpectVoid();
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
IBuffer::ExpectVoid FileBuffer::_copyFrom(FileBuffer& src)
{
  std::fstream& fsSrc = src._fs;
  char buffer[_copyBufferSize];
  while (!fsSrc.eof()) {
    fsSrc.read(buffer, _copyBufferSize);
    if (fsSrc.bad()) {
      return ft::unexpected<BufferException>(errRead);
    }
    const std::streamsize bytesRead = fsSrc.gcount();
    if (bytesRead > 0) {
      _fs.write(buffer, bytesRead);
      if (_fs.fail()) {
        return ft::unexpected<BufferException>(errWrite);
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
    return ft::unexpected<BufferException>(errOpen);
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
