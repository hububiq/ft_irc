#ifndef EXECUTOR_COMMANDS_INVITE_HPP
#define EXECUTOR_COMMANDS_INVITE_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "reply_factory.hpp"
#include "validator.hpp"

class Validator;

class Invite : public ACommand {
 private:
  Validator* m_validator;
 public:
  Invite(ServerDao *server = NULL, Validator *validator = NULL);
  
  
  
  void execute(Client&, Message&);
};

#endif
