#pragma once
#ifndef SMART_BUFFER_HPP
#define SMART_BUFFER_HPP

#include <libftpp/memory.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/IInOutBuffer.hpp>

#include <cstddef>
#include <string>
#include <sys/types.h>

/* ************************************************************************** */
class SmartBuffer : public IInOutBuffer
{
public:
  static const char* const errAllocBuffer;

  SmartBuffer();
  ~SmartBuffer() {}

  static std::size_t getMemoryToFileThreshold();
  static std::size_t getFileToMemoryThreshold();
  static void setMemoryToFileThreshold(std::size_t value);
  static void setFileToMemoryThreshold(std::size_t value);

  // Interface IInOutBuffer - Throwing versions
  char get();
  char peek();
  void seek(std::size_t pos);
  std::size_t pos();
  void append(const std::string& data);
  void append(const RawBytes& buffer, std::size_t bytes);
  void removeFront(std::size_t bytes);
  std::string consumeFront(std::size_t bytes);
  RawBytes consumeRawFront(std::size_t bytes);
  RawBytes consumeAll();
  std::string getStr(std::size_t start, std::size_t bytes);
  RawBytes getRawBytes(std::size_t start, std::size_t bytes);
  void replace(RawBytes& rawData);
  void moveBufferToFile(const std::string& filepath);
  void reset();

  bool isEmpty() const;
  std::size_t size() const;
  ssize_t send(int fdes, std::size_t bytes);

  // Non-interface
  bool usesFile() const;

protected:
  const IInBuffer* getRawBuffer() const;

private:
  SmartBuffer(const SmartBuffer& other);
  SmartBuffer& operator=(const SmartBuffer& other);

  static const std::size_t _defaultMemoryToFileThreshold = 4096;
  static const std::size_t _defaultFileToMemoryThreshold = 1024;
  static std::size_t _memoryToFileThreshold;
  static std::size_t _fileToMemoryThreshold;

  bool _fileNeeded(std::size_t newBytes);
  bool _memoryBufferPossible();
  void _switchToFileBuffer();
  void _switchToMemoryBuffer();

  ft::unique_ptr<IInOutBuffer> _buffer;
  bool _usesFile;
};

#endif
