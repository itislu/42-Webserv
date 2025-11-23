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
  SmartBuffer(const SmartBuffer& other);
  SmartBuffer& operator=(const SmartBuffer& other);

  static const std::size_t _thresholdMemoryBuffer = 0; // always uses file now

  bool _fileNeeded(long newBytes);
  void _switchToFileBuffer();

  ft::unique_ptr<IBuffer> _buffer;
  bool _usesFile;
};

#endif
