#pragma once
#ifndef STATIC_FILE_BUFFER_HPP
#define STATIC_FILE_BUFFER_HPP

#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>

#include <cstddef>
#include <fstream>
#include <new>
#include <string>

/* ************************************************************************** */
class StaticFileBuffer : public IInBuffer
{
public:
  static const char* const errOpen;
  static const char* const errSeek;
  static const char* const errFileEmpty;
  static const char* const errOutOfRange;
  static const char* const errRead;
  static const char* const errWrite;
  static const char* const errTell;

  StaticFileBuffer();
  explicit StaticFileBuffer(const std::string& filepath);
  ~StaticFileBuffer();

  // Interface IInBuffer - Throwing versions
  char get();
  char peek();
  void seek(std::size_t pos);
  std::size_t pos();
  std::string consumeFront(std::size_t bytes);
  RawBytes consumeRawFront(std::size_t bytes);
  RawBytes consumeAll();
  std::string getStr(std::size_t start, std::size_t bytes);
  RawBytes getRawBytes(std::size_t start, std::size_t bytes);

  // Interface IInBuffer - Non-throwing versions
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

  bool isEmpty() const;
  std::size_t size() const;

private:
  StaticFileBuffer(const StaticFileBuffer& other);
  StaticFileBuffer& operator=(const StaticFileBuffer& other);

  char _getChr(std::fstream::int_type (std::fstream::*func)());
  template<typename ContigContainer>
  ContigContainer _consumeFront(std::size_t bytes);
  template<typename ContigContainer>
  ContigContainer _getData(std::size_t start, std::size_t bytes);

  std::fstream _fs;
  std::string _fileName;
  std::size_t _size;
  std::size_t _consumendFront;
};

#endif
