#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "Client.hpp"
#include "ClientState.hpp"
#include "CommandType.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "CommandHandler.hpp"
#include "reply_factory.hpp"

class CommandHandler;

class Executor {
 public:
  Executor(CommandHandler *commandHandler);
  ~Executor();
  void executeMessage(Client &client, Message &msg);

 private:
  CommandHandler *m_commandHandler;
  bool process_connected(Client &client, Message &msg);
  bool process_handshake(Client &client, Message &msg);
  bool process_registered(Client &client, Message &msg);
  bool validateClientState(Client &client, Message &msg);
};

#endif