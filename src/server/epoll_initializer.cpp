
#include "epoll_initializer.hpp"

#include <stdexcept>
#include <unistd.h>

namespace {
void register_socket(int epoll_fd, int socket_fd) {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = socket_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1) {
    close(epoll_fd);
    throw std::runtime_error("Failed to add socket to epoll.");
  }
}
}  // namespace

int epoll_initializer::init_epoll(int socket_fd) {
  int epoll_fd = epoll_create(1);
  if (epoll_fd == -1) {
    throw std::runtime_error("Failed to create epoll file descriptor.");
  }
  register_socket(epoll_fd, socket_fd);
  return epoll_fd;
}