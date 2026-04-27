#ifndef EXECUTOR_COMMANDS_JOIN_HPP
#define EXECUTOR_COMMANDS_JOIN_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "join_gatekeeper.hpp"
#include "reply_factory.hpp"
#include "validator.hpp"

class Validator;
class JoinGatekeeper;

class Join : public ACommand {
 private:
  Validator* m_validator;
  JoinGatekeeper* m_joinGatekeeper;
 public:
  Join(ServerDao *server = NULL, Validator *validator = NULL, JoinGatekeeper *joinGatekeeper = NULL);
  
  
  
  void execute(Client&, Message&);
};

#endif
