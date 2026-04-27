#ifndef CONN_HANDLER_HPP
#define CONN_HANDLER_HPP

#include "Client.hpp"
#include "ServerDao.hpp"

namespace conn_handler {
void process_connect(int socket_fd);
}  // namespace conn_handler

#endif
