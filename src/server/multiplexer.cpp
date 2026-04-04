#include "server.hpp"

/*
  monitor INCOMING data (EPOLLIN)
*/
void register_socket(int epoll_fd, int socket_fd) {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = socket_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1) {
    throw std::runtime_error("Failed to add socket to epoll.");
  }
}

int multiplexer::init_epoll(Server& server) {
  int epoll_fd = epoll_create(1);
  if (epoll_fd == -1) {
    throw std::runtime_error("Failed to create epoll file descriptor.");
  }
  register_socket(epoll_fd, server.getSocketFd());
  return epoll_fd;
}

void multiplexer::loop_epoll(int epoll_fd, Server& server) {
  struct epoll_event events[LIMIT];
  std::map<int, Client> clients;
  while (true) {
    int num_ready = epoll_wait(epoll_fd, events, LIMIT, TIMEOUT);
    for (int i = 0; i < num_ready; i++) {
      int event_fd = events[i].data.fd;
      if (event_fd == server.getSocketFd()) {
        int client_fd = request_handler::process_connect(epoll_fd, event_fd);
        if (client_fd >= 0) {
          Client c(client_fd);
          clients.insert(std::make_pair(client_fd, c));
          std::cout << "Client connected" << std::endl;
        }
      } else if (clients.find(event_fd) != clients.end()) {
        if (request_handler::process_request(
                epoll_fd, events[i].events, clients.find(event_fd)->second) == DROP_CONNECTION) {
          std::cout << "Client disconnected" << std::endl;
          clients.erase(event_fd);
          close(event_fd);
        }
      }
    }
  }
}