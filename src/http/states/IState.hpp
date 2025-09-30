#pragma once
#ifndef I_STATE_HPP
#define I_STATE_HPP

class Client;

/* ************************************************************************** */
class IState
{
public:
  explicit IState(Client* client);
  virtual ~IState();
  virtual void run() = 0;

  Client* getClient() const;

private:
  IState();
  IState(const IState& other);
  IState& operator=(const IState& other);

  Client* _client;
};

#endif // I_STATE_HPP
