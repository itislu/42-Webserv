#pragma once
#ifndef BUFFER_READER_HPP
#define BUFFER_READER_HPP

#include "utils/IBuffer.hpp"

#include <cstddef>
#include <new>

/* ************************************************************************** */
class BufferReader
{
public:
  typedef IBuffer::ExpectChr ExpectChr;
  typedef IBuffer::ExpectVoid ExpectVoid;

  void init(IBuffer* buffer);

  // Throwing versions
  char getNextChar();
  std::size_t getPosInBuff() const;
  void setPosInBuff(std::size_t pos);
  void resetPosInBuff();
  void rewind(std::size_t bytes);

  // Non-throwing versions
  ExpectChr getNextChar(std::nothrow_t /*unused*/);
  ExpectVoid setPosInBuff(std::size_t pos, std::nothrow_t /*unused*/);
  ExpectVoid resetPosInBuff(std::nothrow_t /*unused*/);
  ExpectVoid rewind(std::size_t bytes, std::nothrow_t /*unused*/);

  bool reachedEnd() const;

private:
  IBuffer* _buffer;
  std::size_t _posInBuff;
};

#endif
