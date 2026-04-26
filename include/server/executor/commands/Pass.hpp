#ifndef EXECUTOR_COMMANDS_PASS_HPP
#define EXECUTOR_COMMANDS_PASS_HPP

#include "ACommand.hpp"
extern Server *g_server;

class Pass : public ACommand {
 public:
  void execute(Client &, Message &);
};

#endif