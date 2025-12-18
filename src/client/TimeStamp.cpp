#include "TimeStamp.hpp"
#include "libftpp/numeric.hpp"
#include "libftpp/utility.hpp"
#include <algorithm>
#include <climits>
#include <ctime>

int convertSecondsToMs(long timeoutSecs)
{
  const long ms_mult = 1000L;
  timeoutSecs = std::max(timeoutSecs, 0L);
  const long timeoutMs = ft::mul_sat(timeoutSecs, ms_mult);
  return static_cast<int>(std::min(timeoutMs, static_cast<long>(INT_MAX)));
}

TimeStamp::TimeStamp()
  : _timeStamp(std::time(FT_NULLPTR))
{
}

std::time_t TimeStamp::getTime() const
{
  return _timeStamp;
}

void TimeStamp::setTime(std::time_t time)
{
  _timeStamp = time;
}

std::time_t TimeStamp::now()
{
  return std::time(FT_NULLPTR);
}

std::time_t TimeStamp::operator-(const TimeStamp& other) const
{
  return getTime() - other.getTime();
}
