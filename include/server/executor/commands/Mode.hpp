#ifndef EXECUTOR_COMMANDS_MODE_HPP
#define EXECUTOR_COMMANDS_MODE_HPP

#include "ACommand.hpp"

class Mode : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};

#endif