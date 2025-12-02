#include "BufferQueue.hpp"

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>

#include <cstddef>
#include <sys/types.h>

/* ************************************************************************** */
// INIT

Logger& BufferQueue::_log = Logger::getInstance(LOG_SERVER);

/* ************************************************************************** */
// PUBLIC

void BufferQueue::append(const ft::shared_ptr<IInBuffer>& buffer)
{
  _queue.push_back(buffer);
}

ssize_t BufferQueue::send(int fdes, std::size_t bytes)
{
  if (_queue.empty()) {
    return 0;
  }

  if (_queue.front()->size() == 0) {
    _queue.pop_front();
    _log.warning() << "BufferQueue: empty buffer found\n";
    // One skip should be enough, should only skip empty body files for example
    if (_queue.empty() || _queue.front()->size() == 0) {
      _log.warning() << "BufferQueue: next buffer is still empty\n";
      return 0;
    }
  }
  const ssize_t bytesSent = _queue.front()->send(fdes, bytes);
  if (_queue.front()->isEmpty()) {
    _queue.pop_front();
  }
  return bytesSent;
}

bool BufferQueue::isEmpty() const
{
  return _queue.empty();
}

// NOLINTBEGIN(misc-const-correctness)
SmartBuffer* BufferQueue::getSmartBuffer()
{
  if (!_queue.empty()) {
    const ft::shared_ptr<IInBuffer>& base = _queue.back();
    IInBuffer* const raw = base.get();
    SmartBuffer* const derived = dynamic_cast<SmartBuffer*>(raw);
    if (derived != FT_NULLPTR) {
      return derived;
    }
  }

  const ft::shared_ptr<SmartBuffer> sbuff = ft::make_shared<SmartBuffer>();
  _queue.push_back(sbuff);
  return sbuff.shared_ptr<SmartBuffer>::get();
}
// NOLINTEND(misc-const-correctness)

/* ************************************************************************** */
// PRIVATE
