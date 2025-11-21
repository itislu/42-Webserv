#pragma once
#ifndef SMART_BUFFER_HPP
#define SMART_BUFFER_HPP

#include <libftpp/memory.hpp>
#include <utils/IBuffer.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
class SmartBuffer : public IBuffer
{
public:
  static const char* const errAllocBuffer;

  SmartBuffer();
  ~SmartBuffer() {}

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
  SmartBuffer(const SmartBuffer& other);
  SmartBuffer& operator=(const SmartBuffer& other);

  static const std::size_t _thresholdMemoryBuffer = 0; // always uses file now

  void _checkBuffer();
  bool _fileNeeded(long newBytes);
  ExpectVoid _switchToFileBuffer();

  ft::shared_ptr<IBuffer> _buffer;
  bool _usesFile;
};

#endif
