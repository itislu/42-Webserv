#pragma once
#ifndef FILE_BUFFER_HPP
#define FILE_BUFFER_HPP

#include <libftpp/expected.hpp>
#include <utils/IBuffer.hpp>

#include <cstddef>
#include <fstream>
#include <ios>
#include <string>

/* ************************************************************************** */
class FileBuffer : public IBuffer
{
public:
  static const char* const errOpen;
  static const char* const errSeek;
  static const char* const errFileEmpty;
  static const char* const errOutOfRange;
  static const char* const errRead;
  static const char* const errWrite;

  FileBuffer();
  ~FileBuffer();

  // Interface IBuffer
  ExpectChr get();
  ExpectChr peek();
  ExpectVoid seek(std::size_t pos);
  ExpectVoid append(const std::string& data);
  ExpectVoid append(const RawBytes& buffer, long bytes);
  ExpectVoid removeFront(std::size_t bytes);
  ExpectStr consumeFront(std::size_t bytes);
  ExpectRaw consumeRawFront(std::size_t bytes);
  ExpectRaw consumeAll();
  ExpectStr getStr(std::size_t start, std::size_t bytes);
  ExpectRaw getRawBytes(std::size_t start, std::size_t bytes);
  ExpectVoid replace(RawBytes& rawData);
  bool isEmpty() const;
  std::size_t size() const;
  std::size_t pos();

private:
  FileBuffer(const FileBuffer& other);
  FileBuffer& operator=(const FileBuffer& other);

  ExpectVoid _openTmpFile();
  ExpectChr _getChr(std::fstream::int_type (std::fstream::*func)());
  template<typename ContigContainer>
  ft::expected<ContigContainer, BufferException> _consumeFront(
    std::size_t bytes);
  template<typename ContigContainer>
  ft::expected<ContigContainer, BufferException> _getData(std::size_t bytes);
  template<typename ContigContainer>
  ft::expected<ContigContainer, BufferException> _getDataRange(
    std::size_t start,
    std::size_t bytes);
  ExpectVoid _append(const char* data, std::streamsize bytes);
  ExpectVoid _saveRemainder();
  ExpectVoid _copyFrom(FileBuffer& src);
  ExpectVoid _replaceCurrFile(FileBuffer& tmpFb);
  void _removeCurrFile();

  static const std::size_t _copyBufferSize = 4096;

  std::fstream _fs;
  std::string _fileName;
  std::size_t _size;
};

#endif
