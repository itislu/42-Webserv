#pragma once
#ifndef BUFFER_READER_HPP
#define BUFFER_READER_HPP

#include "utils/IBuffer.hpp"

/* ************************************************************************** */
class BufferReader
{
public:
  void init(IBuffer* buffer);

  bool reachedEnd() const;
  char getNextChar();
  long getPosInBuff() const;
  void setPosInBuff(long pos);
  void resetPosInBuff();
  void rewind(long bytes);

  void printRemaining();

  bool fail() const;
  const IBuffer::BufferException& error() const;

private:
  IBuffer* _buffer;
  IBuffer::BufferException _error;
  long _posInBuff;
  bool _fail;
};

#endif
