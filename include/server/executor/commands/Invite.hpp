#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"
#include "validator.hpp"

#ifndef EXECUTOR_COMMANDS_INVITE_HPP
#define EXECUTOR_COMMANDS_INVITE_HPP

#include "ACommand.hpp"

class Invite : public ACommand {
 public:
  void execute(Client&, Message&, Server&);
};

#endif