#pragma once
#ifndef FILE_BUFFER_HPP
#define FILE_BUFFER_HPP

#include <utils/buffer/IInOutBuffer.hpp>

#include <cstddef>
#include <fstream>
#include <ios>
#include <new>
#include <string>

/* ************************************************************************** */
class FileBuffer : public IInOutBuffer
{
public:
  static const char* const errOpen;
  static const char* const errSeek;
  static const char* const errFileEmpty;
  static const char* const errOutOfRange;
  static const char* const errRead;
  static const char* const errWrite;
  static const char* const errTell;
  static const char* const errRename;

  FileBuffer();
  ~FileBuffer();

  // Interface IInOutBuffer - Throwing versions
  char get();
  char peek();
  void seek(std::size_t pos);
  std::size_t pos();
  std::string consumeFront(std::size_t bytes);
  RawBytes consumeRawFront(std::size_t bytes);
  RawBytes consumeAll();
  std::string getStr(std::size_t start, std::size_t bytes);
  RawBytes getRawBytes(std::size_t start, std::size_t bytes);
  void append(const std::string& data);
  void append(const RawBytes& buffer, std::size_t bytes);
  void removeFront(std::size_t bytes);
  void replace(RawBytes& rawData);
  void moveBufferToFile(const std::string& filepath);

  // Interface IInOutBuffer - Non-throwing versions
  ExpectChr get(std::nothrow_t /*unused*/);
  ExpectChr peek(std::nothrow_t /*unused*/);
  ExpectVoid seek(std::size_t pos, std::nothrow_t /*unused*/);
  ExpectPos pos(std::nothrow_t /*unused*/);
  ExpectStr consumeFront(std::size_t bytes, std::nothrow_t /*unused*/);
  ExpectRaw consumeRawFront(std::size_t bytes, std::nothrow_t /*unused*/);
  ExpectRaw consumeAll(std::nothrow_t /*unused*/);
  ExpectStr getStr(std::size_t start,
                   std::size_t bytes,
                   std::nothrow_t /*unused*/);
  ExpectRaw getRawBytes(std::size_t start,
                        std::size_t bytes,
                        std::nothrow_t /*unused*/);
  ExpectVoid append(const std::string& data, std::nothrow_t /*unused*/);
  ExpectVoid append(const RawBytes& buffer,
                    std::size_t bytes,
                    std::nothrow_t /*unused*/);
  ExpectVoid removeFront(std::size_t bytes, std::nothrow_t /*unused*/);
  ExpectVoid replace(RawBytes& rawData, std::nothrow_t /*unused*/);
  ExpectVoid moveBufferToFile(const std::string& filepath,
                              std::nothrow_t /*unused*/);

  bool isEmpty() const;
  std::size_t size() const;

private:
  FileBuffer(const FileBuffer& other);
  FileBuffer& operator=(const FileBuffer& other);

  void _openTmpFile();
  char _getChr(std::fstream::int_type (std::fstream::*func)());
  template<typename ContigContainer>
  ContigContainer _consumeFront(std::size_t bytes);
  template<typename ContigContainer>
  ContigContainer _getData(std::size_t start, std::size_t bytes);
  void _append(const char* data, std::streamsize bytes);
  void _saveRemainder();
  void _copyFrom(FileBuffer& src);
  void _replaceCurrFile(FileBuffer& tmpFb);
  void _removeCurrFile();

  static const std::size_t _copyBufferSize = 4096;

  std::fstream _fs;
  std::string _fileName;
  std::size_t _size;
};

#endif
