#ifndef EXECUTOR_COMMANDS_USER_HPP
#define EXECUTOR_COMMANDS_USER_HPP

#include "ACommand.hpp"

class User : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};

#endif