#include "BufferQueue.hpp"

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>

#include <algorithm>
#include <cstddef>
#include <stdexcept>

/* ************************************************************************** */
// INIT

Logger& BufferQueue::_log = Logger::getInstance(LOG_SERVER);

/* ************************************************************************** */
// PUBLIC

void BufferQueue::append(const ft::shared_ptr<IInBuffer>& buffer)
{
  _queue.push_back(buffer);
}

IBuffer::RawBytes BufferQueue::read(std::size_t bytes)
{
  if (_queue.empty()) {
    throw std::runtime_error("BufferQueue: queue is empty");
  }
  if (_queue.front()->size() == 0) {
    _queue.pop_front();
    _log.warning() << "BufferQueue: empty buffer found\n";
    return IBuffer::RawBytes();
  }
  const std::size_t toRead = std::min(bytes, _queue.front()->size());
  const IBuffer::RawBytes rawBytes = _queue.front()->consumeRawFront(toRead);
  if (_queue.front()->isEmpty()) {
    _queue.pop_front();
  }
  return rawBytes;
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
