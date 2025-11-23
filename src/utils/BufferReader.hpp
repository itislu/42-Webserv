#pragma once
#ifndef BUFFER_READER_HPP
#define BUFFER_READER_HPP

#include "utils/IBuffer.hpp"

#include <cstddef>

/* ************************************************************************** */
class BufferReader
{
public:
  void init(IBuffer* buffer);

  bool reachedEnd() const;
  char getNextChar();
  std::size_t getPosInBuff() const;
  void setPosInBuff(std::size_t pos);
  void resetPosInBuff();
  void rewind(std::size_t bytes);

private:
  IBuffer* _buffer;
  std::size_t _posInBuff;
};

#endif
