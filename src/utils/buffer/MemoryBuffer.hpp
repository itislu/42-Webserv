#pragma once
#ifndef MEMORY_BUFFER_HPP
#define MEMORY_BUFFER_HPP

#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInOutBuffer.hpp>

#include <cstddef>
#include <string>
#include <sys/types.h>

class MemoryBuffer : public IInOutBuffer
{
public:
  static const char* const errOpen;
  static const char* const errOutOfRange;
  static const char* const errBufferEmpty;
  static const char* const errWrite;

  MemoryBuffer();
  explicit MemoryBuffer(const std::string& data);
  explicit MemoryBuffer(const RawBytes& buffer, std::size_t bytes);
  ~MemoryBuffer();

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

  bool isEmpty() const;
  std::size_t size() const;
  ssize_t send(int fdes, std::size_t bytes);

private:
  MemoryBuffer(const MemoryBuffer& other);
  MemoryBuffer& operator=(const MemoryBuffer& other);

  template<typename ContigContainer>
  ContigContainer _consumeFront(std::size_t bytes);
  template<typename ContigContainer>
  ContigContainer _getData(std::size_t start, std::size_t bytes);

  static const int _startSize = 4096;

  IBuffer::RawBytes _data;
  std::size_t _pos;
};

#endif
