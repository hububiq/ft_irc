#ifndef EXECUTOR_COMMANDS_KICK_HPP
#define EXECUTOR_COMMANDS_KICK_HPP

#include <string>

#include "ACommand.hpp"

class Channel;
class Client;

class Kick : public ACommand
{
  public:
	Kick(ServerDao *server = NULL);

	void    execute(Client &, Message &);
	bool    clientCanKICK(Channel *, Client &);
	Client *clientToBeKICKed(Channel *, const std::string &);
};

// Client *clientToBeKICKed(Channel *, const std::string &);
// bool clientCanKICK(Channel *, Client &);

#endif
