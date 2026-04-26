#ifndef EPOLL_INITIALIZER_HPP
#define EPOLL_INITIALIZER_HPP

#include <sys/epoll.h>

namespace epoll_initializer {
int init_epoll(int socket_fd);
}  // namespace epoll_initializer

#endif
