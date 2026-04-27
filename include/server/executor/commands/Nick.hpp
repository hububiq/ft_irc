#ifndef EXECUTOR_COMMANDS_NICK_HPP
#define EXECUTOR_COMMANDS_NICK_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "reply_factory.hpp"
#include "validator.hpp"

class Nick : public ACommand {
 public:
  Nick(ServerDao *server = NULL);
  
  
  
  void execute(Client&, Message&);
};

#endif
