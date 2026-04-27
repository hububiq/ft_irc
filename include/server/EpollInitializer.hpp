#ifndef EPOLL_INITIALIZER_HPP
#define EPOLL_INITIALIZER_HPP

#include <sys/epoll.h>

class EpollInitializer
{
  public:
	int init_epoll(int socket_fd);

  private:
	void register_socket(int epoll_fd, int socket_fd);
};

#endif
