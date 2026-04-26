#ifndef EXECUTOR_COMMANDS_MODE_HPP
#define EXECUTOR_COMMANDS_MODE_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"

#include "mode_reporter.hpp"


class Mode : public ACommand {
 public:
  void execute(Client&, Message&);
};

#endif