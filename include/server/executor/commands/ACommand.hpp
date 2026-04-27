#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <cstddef>

class Client;
class ServerDao;
struct Message;

class ACommand
{
  protected:
	ServerDao *m_server;

  public:
	ACommand(ServerDao *server = NULL) : m_server(server)
	{
	}
	virtual ~ACommand() {};
	virtual void execute(Client &, Message &) = 0;
};

#endif
