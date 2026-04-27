#ifndef EXECUTOR_COMMANDS_USER_HPP
#define EXECUTOR_COMMANDS_USER_HPP

#include <algorithm>

#include "ACommand.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "reply_factory.hpp"

class User : public ACommand
{
  public:
	User(ServerDao *server = NULL);

	void execute(Client &, Message &);
};

#endif
