#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <arpa/inet.h>

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

#include "ServerDao.hpp"
#include "epoll_initializer.hpp"
#include "listener.hpp"
#include "multiplexer.hpp"

class MessageParser;
class CommandHandler;
class Executor;

class ServerRunner {
public:
  ServerRunner(int argc, char **argv);
  ~ServerRunner();
  void run();

private:
  ServerDao *m_server;
  Listener m_listener;
  EpollInitializer m_epollInitializer;
  Multiplexer *m_multiplexer;
};

#endif
