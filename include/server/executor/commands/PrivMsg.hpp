#ifndef EXECUTOR_COMMANDS_PRIVMSG_HPP
#define EXECUTOR_COMMANDS_PRIVMSG_HPP

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "reply_factory.hpp"

class PrivMsg : public ACommand
{
  public:
	PrivMsg(ServerDao *server = NULL);

	void execute(Client &, Message &);
};

#endif
