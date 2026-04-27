#ifndef EXECUTOR_COMMANDS_CAP_HPP
#define EXECUTOR_COMMANDS_CAP_HPP

#include "ACommand.hpp"

class Cap : public ACommand {
 public:
  Cap(ServerDao *server = NULL);
  
  
  
  void execute(Client&, Message&);
};

#endif
