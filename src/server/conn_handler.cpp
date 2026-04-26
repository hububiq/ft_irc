#include "Server.hpp"

void Server::register_client(int client_fd, std::string &hostname) {
  Client c(client_fd);
  c.setHostname(hostname);
  c.setState(CONNECTED);
  this->_clients.insert(std::make_pair(client_fd, c));
  std::cout << "Client connected" << std::endl;
}

void Server::process_connect(int epoll_fd, int socket_fd) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);

  int client_fd = accept(socket_fd, (struct sockaddr *)&addr, &len);
  if (client_fd >= 0) {
    std::string hostname = inet_ntoa(addr.sin_addr);
    int flags = fcntl(client_fd, F_GETFL, 0);
    if (flags != -1) {
      fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    }
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = client_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
    register_client(client_fd, hostname);
  }
}