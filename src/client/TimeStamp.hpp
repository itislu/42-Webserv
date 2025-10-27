#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

#include <ctime>
class TimeStamp
{
public:
  TimeStamp();

  std::time_t getTime() const;
  void setTime(std::time_t time);
  std::time_t operator-(const TimeStamp& other) const;

  static std::time_t now();

private:
  std::time_t _timeStamp;
};

// TODO: add print overload so it prints a formated time

int convertSecondsToMs(long timeoutSecs);

#endif
