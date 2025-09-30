#pragma once
#ifndef I_STATE_HPP
#define I_STATE_HPP

#include "client/Client.hpp"

/* ************************************************************************** */
class IState
{
public:
  IState();
  virtual ~IState();
  IState(const IState& other);
  IState& operator=(const IState& other);

  virtual void run(Client& client) = 0;
};

#endif // I_STATE_HPP
