#pragma once
#ifndef BUFFER_READER_HPP
#define BUFFER_READER_HPP

#include <utils/Buffer.hpp>

/* ************************************************************************** */
class BufferReader
{
public:
  void init(Buffer* buffer);
  bool reachedEnd() const;
  char getNextChar();
  char getCurrChar() const;
  long getPosInBuff() const;
  void setPosInBuff(long pos);
  void resetPosInBuff();
  void rewind(long bytes);

  void printRemaining();

private:
  Buffer* _buffer;
  long _posInBuff;
};

#endif // BUFFER_READER_HPP
