#ifndef EXECUTOR_COMMANDS_TOPIC_HPP
#define EXECUTOR_COMMANDS_TOPIC_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"

class Topic : public ACommand {
 public:
  void execute(Client&, Message&);
};

#endif