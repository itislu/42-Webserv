#include "TimeStamp.hpp"
#include "libftpp/utility.hpp"
#include <algorithm>
#include <climits>
#include <ctime>

int convertSecondsToMs(long timeoutSecs)
{
  timeoutSecs = std::max(timeoutSecs, 0L);
  timeoutSecs =
    std::min(timeoutSecs, static_cast<long>(INT_MAX) / MS_MULTIPLIER);
  const long timeoutMs = timeoutSecs * MS_MULTIPLIER;
  return static_cast<int>(timeoutMs);
}

TimeStamp::TimeStamp()
  : _timeStamp(std::time(FT_NULLPTR))
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
  return std::time(FT_NULLPTR);
}

long TimeStamp::operator-(const TimeStamp& other) const
{
  return getTime() - other.getTime();
}
