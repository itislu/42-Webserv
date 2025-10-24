#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

#ifndef MS_MULTIPLIER
#define MS_MULTIPLIER 1000L
#endif

class TimeStamp
{
public:
  TimeStamp();

  long getTime() const;
  void setTime(long time);
  long operator-(const TimeStamp& other) const;

  static long now();

private:
  long _timeStamp;
};

// TODO: add print overload so it prints a formated time

int convertSecondsToMs(long timeoutSecs);

#endif
