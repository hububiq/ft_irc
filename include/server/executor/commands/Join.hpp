#ifndef EXECUTOR_COMMANDS_JOIN_HPP
#define EXECUTOR_COMMANDS_JOIN_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "join_gatekeeper.hpp"
#include "reply_factory.hpp"
#include "validator.hpp"

class Join : public ACommand {
 public:
  void execute(Client&, Message&);
};

#endif