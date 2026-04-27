#ifndef EXECUTOR_COMMANDS_PING_HPP
#define EXECUTOR_COMMANDS_PING_HPP

#include "ACommand.hpp"

class Ping : public ACommand
{
  public:
	Ping(ServerDao *server = NULL);

	void execute(Client &, Message &);
};

#endif
