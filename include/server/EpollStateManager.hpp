#ifndef EPOLL_STATE_MANAGER_HPP
#define EPOLL_STATE_MANAGER_HPP

#include "Client.hpp"
#include "ServerDao.hpp"

class EpollStateManager
{
  public:
	EpollStateManager(ServerDao *server);
	void schedule_epollin(Client &client);
	void schedule_send();

  private:
	ServerDao *m_server;
	void       schedule_epollout(Client &client);
};

#endif
