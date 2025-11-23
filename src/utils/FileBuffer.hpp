#pragma once
#ifndef FILE_BUFFER_HPP
#define FILE_BUFFER_HPP

#include <utils/IBuffer.hpp>

#include <cstddef>
#include <fstream>
#include <ios>
#include <new>
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
  char get();
  ExpectChr get(std::nothrow_t /*unused*/);
  char peek();
  ExpectChr peek(std::nothrow_t /*unused*/);
  void seek(std::size_t pos);
  ExpectVoid seek(std::size_t pos, std::nothrow_t /*unused*/);
  void append(const std::string& data);
  ExpectVoid append(const std::string& data, std::nothrow_t /*unused*/);
  void append(const RawBytes& buffer, long bytes);
  ExpectVoid append(const RawBytes& buffer,
                    long bytes,
                    std::nothrow_t /*unused*/);
  void removeFront(std::size_t bytes);
  ExpectVoid removeFront(std::size_t bytes, std::nothrow_t /*unused*/);
  std::string consumeFront(std::size_t bytes);
  ExpectStr consumeFront(std::size_t bytes, std::nothrow_t /*unused*/);
  RawBytes consumeAll();
  ExpectRaw consumeAll(std::nothrow_t /*unused*/);
  std::string getStr(std::size_t start, std::size_t bytes);
  ExpectStr getStr(std::size_t start,
                   std::size_t bytes,
                   std::nothrow_t /*unused*/);
  RawBytes getRawBytes(std::size_t start, std::size_t bytes);
  ExpectRaw getRawBytes(std::size_t start,
                        std::size_t bytes,
                        std::nothrow_t /*unused*/);
  void replace(RawBytes& rawData);
  ExpectVoid replace(RawBytes& rawData, std::nothrow_t /*unused*/);

  bool isEmpty() const;
  std::size_t size() const;
  void print();

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
