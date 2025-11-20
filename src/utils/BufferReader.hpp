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

private:
  IBuffer* _buffer;
  long _posInBuff;
};

#endif
