
#ifndef EXECUTOR_COMMANDS_QUIT_HPP
#define EXECUTOR_COMMANDS_QUIT_HPP

#include "ACommand.hpp"

class Quit : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};

#endif
