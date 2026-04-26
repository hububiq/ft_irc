#ifndef EXECUTOR_COMMANDS_PRIVMSG_HPP
#define EXECUTOR_COMMANDS_PRIVMSG_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"

class PrivMsg : public ACommand {
 public:
  void execute(Client&, Message&);
};

#endif