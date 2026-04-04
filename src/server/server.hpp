#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Client.hpp"
#include "ClientStatus.hpp"
#include "EpollConfig.hpp"
#include "Server.hpp"

class Server;
class Client;

namespace listener {
void init_socket(Server& server);
}

namespace multiplexer {
int init_epoll(Server& server);
void loop_epoll(int epoll_fd, Server& server);
}  // namespace multiplexer

enum HandleResult { KEEP_CONNECTION, DROP_CONNECTION };

namespace request_handler {
int process_connect(int epoll_fd, int socket_fd);
HandleResult process_request(int epoll_fd, uint32_t events, Client& client);
}  // namespace request_handler

namespace responder {
HandleResult respond(Client& client);
}

#endif