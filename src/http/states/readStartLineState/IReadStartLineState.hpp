#pragma once
#ifndef I_READ_START_LINE_STATE_HPP
#define I_READ_START_LINE_STATE_HPP

class Client;
class ReadStartLine;

/* ************************************************************************** */
class IReadStartLineState
{
public:
  explicit IReadStartLineState(ReadStartLine* state);
  virtual ~IReadStartLineState();

  virtual void run() = 0;
  ReadStartLine* getState() const;

private:
  IReadStartLineState();
  IReadStartLineState(const IReadStartLineState& other);
  IReadStartLineState& operator=(const IReadStartLineState& other);

  ReadStartLine* _state;
};

#endif // I_READ_START_LINE_STATE_HPP
