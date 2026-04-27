#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <stdint.h>

#include "ServerDao.hpp"
#include "conn_handler.hpp"
#include "epoll_state_manager.hpp"
#include "request_handler.hpp"

class Multiplexer {
public:
  Multiplexer(ServerDao *server, ConnHandler *connHandler,
              RequestHandler *requestHandler, EpollStateManager *stateManager);
  ~Multiplexer();
  void loop_epoll();
private:
  ServerDao *m_server;
  ConnHandler *m_connHandler;
  RequestHandler *m_requestHandler;
  EpollStateManager *m_stateManager;
};

#endif
