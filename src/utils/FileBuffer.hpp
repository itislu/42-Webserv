#pragma once
#ifndef FILE_BUFFER_HPP
#define FILE_BUFFER_HPP

#include <utils/IBuffer.hpp>

#include <cstddef>
#include <fstream>
#include <string>

/* ************************************************************************** */
class FileBuffer : public IBuffer
{
public:
  static const char* const errOpen;
  static const char* const errSeek;
  static const char* const errFileEmpty;
  static const char* const errFileNotOpened;
  static const char* const errEof;
  static const char* const errRead;
  static const char* const errWrite;

  FileBuffer();
  ~FileBuffer();

  // Interface IBuffer
  ExpectChr get();
  ExpectChr peek();
  ExpectVoid seek(std::size_t pos);
  ExpectVoid append(const std::string& data);
  ExpectVoid append(const Container& buffer, long bytes);
  ExpectVoid removeFront(std::size_t bytes);
  ExpectStr consumeFront(std::size_t bytes);
  std::size_t size() const;

private:
  FileBuffer(const FileBuffer& other);
  FileBuffer& operator=(const FileBuffer& other);

  ExpectVoid _openTmpFile();
  ExpectStr _getFront(std::size_t bytes);
  ExpectVoid _saveRemainder();
  static ExpectVoid _copyData(std::fstream& bufFrom, std::fstream& bufTo);
  ExpectVoid _replaceCurrFile(FileBuffer& tmpFb);

  static const std::size_t _copyBufferSize = 4096;

  std::fstream _fs;
  std::string _fileName;
  std::size_t _size;
};

#endif
