#include "TimeStamp.hpp"
#include <algorithm>
#include <climits>
#include <ctime>

int convertToMs(long timeout)
{
  long timeoutMs = timeout * MS_MULTIPLIER;
  timeoutMs = std::min(timeoutMs, static_cast<long>(INT_MAX));
  timeoutMs = std::max(timeoutMs, 0L);
  return static_cast<int>(timeoutMs);
}

TimeStamp::TimeStamp()
  : _timeStamp(std::time(NULL))
{
}

long TimeStamp::getTime() const
{
  return _timeStamp;
}

void TimeStamp::setTime(long time)
{
  _timeStamp = time;
}

long TimeStamp::now()
{
  return std::time(NULL);
}

long TimeStamp::operator-(const TimeStamp& other) const
{
  return getTime() - other.getTime();
}
