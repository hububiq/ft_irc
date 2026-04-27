
#ifndef EXECUTOR_COMMANDS_QUIT_HPP
#define EXECUTOR_COMMANDS_QUIT_HPP

#include <algorithm>

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "reply_factory.hpp"

class Quit : public ACommand
{
  public:
	Quit(ServerDao *server = NULL);

	void execute(Client &, Message &);
};

#endif
