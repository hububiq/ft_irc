#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <string>

#include "Cap.hpp"
#include "Client.hpp"
#include "Invite.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Message.hpp"
#include "Mode.hpp"
#include "Nick.hpp"
#include "Pass.hpp"
#include "Ping.hpp"
#include "PrivMsg.hpp"
#include "Quit.hpp"
#include "ServerDao.hpp"
#include "Topic.hpp"
#include "User.hpp"
#include "reply_factory.hpp"

class ACommand;
class Client;
class ServerDao;
struct Message;

class CommandHandler {
 private:
  std::map<std::string, ACommand*> _commandsMap;

 public:
  CommandHandler(ServerDao *server);
  ~CommandHandler();
  void handleCommand(Client& client, Message& msg);
};

#endif
