#include "TimeStamp.hpp"
#include <ctime>

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
