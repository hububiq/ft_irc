#ifndef EXECUTOR_COMMANDS_NICK_HPP
#define EXECUTOR_COMMANDS_NICK_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "Validator.hpp"
#include "reply_factory.hpp"

class Validator;

class Nick : public ACommand
{
  private:
	Validator *m_validator;

  public:
	Nick(ServerDao *server = NULL, Validator *Validator = NULL);

	void execute(Client &, Message &);
};

#endif
