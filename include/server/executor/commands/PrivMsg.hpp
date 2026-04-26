#ifndef EXECUTOR_COMMANDS_PRIVMSG_HPP
#define EXECUTOR_COMMANDS_PRIVMSG_HPP

#include "ACommand.hpp"

class PrivMsg : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};

#endif