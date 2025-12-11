#pragma once
#ifndef STATIC_FILE_BUFFER_HPP
#define STATIC_FILE_BUFFER_HPP

#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>

#include <cstddef>
#include <fstream>
#include <string>
#include <sys/types.h>

/* ************************************************************************** */
class StaticFileBuffer : public IInBuffer
{
public:
  static const char* const errOpen;
  static const char* const errSeek;
  static const char* const errFileEmpty;
  static const char* const errOutOfRange;
  static const char* const errRead;
  static const char* const errTell;

  explicit StaticFileBuffer(const std::string& filepath);
  ~StaticFileBuffer() {}

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

  bool isEmpty() const;
  std::size_t size() const;
  ssize_t send(int fdes, std::size_t bytes);

private:
  StaticFileBuffer();
  StaticFileBuffer(const StaticFileBuffer& other);
  StaticFileBuffer& operator=(const StaticFileBuffer& other);

  char _getChr(std::fstream::int_type (std::ifstream::*func)());
  template<typename ContigContainer>
  ContigContainer _consumeFront(std::size_t bytes);
  template<typename ContigContainer>
  ContigContainer _getData(std::size_t start, std::size_t bytes);

  std::ifstream _fs;
  std::string _fileName;
  std::size_t _size;
  std::size_t _consumedFront;
};

#endif
