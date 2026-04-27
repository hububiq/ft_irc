#ifndef CONN_HANDLER_HPP
#define CONN_HANDLER_HPP

#include "Client.hpp"
#include "ServerDao.hpp"

class ConnHandler {
public:
  ConnHandler(ServerDao *server);
  void process_connect(int socket_fd);

private:
  ServerDao *m_server;
  void register_client(int client_fd, std::string &hostname);
};

#endif
