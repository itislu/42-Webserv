#pragma once
#ifndef SMART_BUFFER_HPP
#define SMART_BUFFER_HPP

#include <libftpp/memory.hpp>
#include <utils/IBuffer.hpp>

#include <cstddef>
#include <new>
#include <string>

/* ************************************************************************** */
class SmartBuffer : public IBuffer
{
public:
  static const char* const errAllocBuffer;

  SmartBuffer();
  ~SmartBuffer() {}

  // Interface IBuffer - Throwing versions
  char get();
  char peek();
  void seek(std::size_t pos);
  void append(const std::string& data);
  void append(const RawBytes& buffer, long bytes);
  void removeFront(std::size_t bytes);
  std::string consumeFront(std::size_t bytes);
  RawBytes consumeAll();
  std::string getStr(std::size_t start, std::size_t bytes);
  RawBytes getRawBytes(std::size_t start, std::size_t bytes);
  void replace(RawBytes& rawData);

  // Interface IBuffer - Non-throwing versions
  ExpectChr get(std::nothrow_t /*unused*/);
  ExpectChr peek(std::nothrow_t /*unused*/);
  ExpectVoid seek(std::size_t pos, std::nothrow_t /*unused*/);
  ExpectVoid append(const std::string& data, std::nothrow_t /*unused*/);
  ExpectVoid append(const RawBytes& buffer,
                    long bytes,
                    std::nothrow_t /*unused*/);
  ExpectVoid removeFront(std::size_t bytes, std::nothrow_t /*unused*/);
  ExpectStr consumeFront(std::size_t bytes, std::nothrow_t /*unused*/);
  ExpectRaw consumeAll(std::nothrow_t /*unused*/);
  ExpectStr getStr(std::size_t start,
                   std::size_t bytes,
                   std::nothrow_t /*unused*/);
  ExpectRaw getRawBytes(std::size_t start,
                        std::size_t bytes,
                        std::nothrow_t /*unused*/);
  ExpectVoid replace(RawBytes& rawData, std::nothrow_t /*unused*/);

  bool isEmpty() const;
  std::size_t size() const;
  void print();

private:
  SmartBuffer(const SmartBuffer& other);
  SmartBuffer& operator=(const SmartBuffer& other);

  static const std::size_t _thresholdMemoryBuffer = 0; // always uses file now

  bool _fileNeeded(long newBytes);
  void _switchToFileBuffer();

  ft::unique_ptr<IBuffer> _buffer;
  bool _usesFile;
};

#endif
