#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

class TimeStamp
{
public:
  TimeStamp();

  long getTime() const;
  void setTime(long time);

  static long now();

private:
  long _timeStamp;
};

#endif
