#ifndef EXECUTOR_COMMANDS_NICK_HPP
#define EXECUTOR_COMMANDS_NICK_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"
#include "validator.hpp"

class Nick : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};

#endif