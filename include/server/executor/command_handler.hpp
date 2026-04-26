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
#include "Server.hpp"
#include "Topic.hpp"
#include "User.hpp"
#include "reply_factory.hpp"

class ACommand;
class Client;
class Server;
struct Message;

class command_handler {
 private:
  static std::map<std::string, ACommand*> _commandsMap;

 public:
  static void setupCommandsMap();
  static void handleCommand(Client& client, Message& msg);
};

#endif
