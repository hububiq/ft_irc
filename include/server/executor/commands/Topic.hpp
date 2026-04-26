#ifndef EXECUTOR_COMMANDS_TOPIC_HPP
#define EXECUTOR_COMMANDS_TOPIC_HPP

#include "ACommand.hpp"

class Topic : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};

#endif