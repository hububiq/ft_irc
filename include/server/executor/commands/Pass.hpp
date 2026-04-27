#ifndef EXECUTOR_COMMANDS_PASS_HPP
#define EXECUTOR_COMMANDS_PASS_HPP

#include "ACommand.hpp"

class Pass : public ACommand {
 public:
  Pass(ServerDao *server = NULL);

  void execute(Client &, Message &);
};

#endif
