#include "Server.hpp"

extern volatile sig_atomic_t g_running;

int Server::init_epoll() {
  int epoll_fd = epoll_create(1);
  if (epoll_fd == -1) {
    throw std::runtime_error("Failed to create epoll file descriptor.");
  }
  register_socket(epoll_fd);
  return epoll_fd;
}

void Server::register_socket(int epoll_fd) {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = this->_socket_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_socket_fd, &event) == -1) {
    close(epoll_fd);
    throw std::runtime_error("Failed to add socket to epoll.");
  }
}

void Server::loop_epoll() {
  struct epoll_event events[LIMIT];
  while (g_running) {
    int num_ready = epoll_wait(this->_epoll_fd, events, LIMIT, TIMEOUT);
    for (int i = 0; i < num_ready; i++) {
      int event_fd = events[i].data.fd;
      if (event_fd == this->_socket_fd) {
        process_connect(event_fd);
      } else {
        std::map<int, Client>::iterator it = this->_clients.find(event_fd);
        if (it != this->_clients.end()) {
          HandleResult res =
              process_request(events[i].events, it->second);
          if (res == DROP_CONNECTION) {
            std::cout << "Client disconnected" << std::endl;
            this->_clients.erase(it);
            close(event_fd);
          }
        }
      }
    }
    schedule_send();
  }
}