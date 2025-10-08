#include "ReadStartLine.hpp"
#include "ParseMethod.hpp"
#include "utils/StateHandler.hpp"

#include <client/Client.hpp>
#include <utils/IState.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

ReadStartLine::ReadStartLine(Client* client)
  : IState(client)
  , _stateHandler(this)
  , _sizeStartLine(0)
{
  _stateHandler.setState<ParseMethod>();
}

ReadStartLine::~ReadStartLine() {}

/**
 * @brief ### Read the first line of a HTTP Request
 *
 * Example: `GET test/hello_world.html HTTP/1.1\r\n`
 */
void ReadStartLine::run()
{
  (void)_sizeStartLine; // todo: use

  _stateHandler.setStateHasChanged(true);
  while (!_stateHandler.isDone() && _stateHandler.stateHasChanged()) {
    _stateHandler.setStateHasChanged(false);
    _stateHandler.getState()->run();
  }

  if (_stateHandler.isDone()) {
    // TODO set state to ReadHeader
    // std::cout << "ReadStartLine: IsDone\n";
  }
}

StateHandler<ReadStartLine>& ReadStartLine::getStateHandler()
{
  return _stateHandler;
}

/* ************************************************************************** */
// PRIVATE

ReadStartLine::ReadStartLine()
  : IState(NULL)
  , _stateHandler(this)
  , _sizeStartLine(0)
{
}

ReadStartLine::ReadStartLine(const ReadStartLine& other)
  : IState(NULL)
  , _stateHandler(this)
  , _sizeStartLine(0)
{
  *this = other;
}

ReadStartLine& ReadStartLine::operator=(const ReadStartLine& other)
{
  if (this != &other) {
  }
  return *this;
}

// bool ReadStartLine::isDone() const
// {
//   return dynamic_cast<Done<ReadStartLine>*>(_stateHandler.getState()) !=
//   NULL;
// }
