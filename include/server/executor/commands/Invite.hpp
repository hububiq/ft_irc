#ifndef EXECUTOR_COMMANDS_INVITE_HPP
#define EXECUTOR_COMMANDS_INVITE_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "Validator.hpp"
#include "reply_factory.hpp"

class Validator;

class Invite : public ACommand
{
  private:
	Validator *m_validator;

  public:
	Invite(ServerDao *server = NULL, Validator *Validator = NULL);

	void execute(Client &, Message &);
};

#endif
