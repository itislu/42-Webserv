#pragma once
#ifndef BUFFER_READER_HPP
#define BUFFER_READER_HPP

#include "utils/IBuffer.hpp"

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
  void setPosInBuff(long pos);
  void resetPosInBuff();
  void rewind(long bytes);

  // Non-throwing versions
  ExpectChr getNextChar(std::nothrow_t /*unused*/);
  ExpectVoid setPosInBuff(long pos, std::nothrow_t /*unused*/);
  ExpectVoid resetPosInBuff(std::nothrow_t /*unused*/);
  ExpectVoid rewind(long bytes, std::nothrow_t /*unused*/);

  bool reachedEnd() const;
  long getPosInBuff() const;

private:
  IBuffer* _buffer;
  long _posInBuff;
};

#endif
