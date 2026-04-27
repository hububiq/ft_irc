#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <arpa/inet.h>

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

#include "ServerDao.hpp"
#include "EpollInitializer.hpp"
#include "Listener.hpp"
#include "Multiplexer.hpp"

class MessageParser;
class CommandHandler;
class Executor;

class ServerRunner
{
  public:
	ServerRunner(int argc, char **argv);
	~ServerRunner();
	void run();

  private:
  std::string parse_pwd(char *arg);
  long parse_port(char *arg);
	ServerDao       *m_server;
	Listener         m_Listener;
	EpollInitializer m_epollInitializer;
	Multiplexer     *m_Multiplexer;
};

#endif
