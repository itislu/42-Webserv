#pragma once
#ifndef BUFFER_QUEUE_HPP
#define BUFFER_QUEUE_HPP

#include <libftpp/memory.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>

#include <cstddef>
#include <deque>
#include <sys/types.h>

/* ************************************************************************** */
class BufferQueue
{
public:
  void append(const ft::shared_ptr<IInBuffer>& buffer);
  ssize_t send(int fdes, std::size_t bytes);
  bool isEmpty() const;
  SmartBuffer* getSmartBuffer();

private:
  static Logger& _log;

  std::deque<ft::shared_ptr<IInBuffer> > _queue;
};

#endif
