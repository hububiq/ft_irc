#include "epoll_state_manager.hpp"

extern ServerDao *g_server;

namespace {
void schedule_epollout(Client &client) {
  struct epoll_event event = {};
  event.events = EPOLLIN | EPOLLOUT;
  event.data.fd = client.getFd();
  epoll_ctl(g_server->getEpollFd(), EPOLL_CTL_MOD, client.getFd(), &event);
}
}  // namespace

void state_manager::schedule_send() {
  for (std::map<int, Client>::iterator it = g_server->getClients().begin();
       it != g_server->getClients().end(); ++it) {
    if (it->second.getStatus() == READY_TO_RESPOND) {
      schedule_epollout(it->second);
    }
  }
}

void state_manager::schedule_epollin(Client &client) {
  struct epoll_event event = {};
  event.events = EPOLLIN;
  event.data.fd = client.getFd();
  epoll_ctl(g_server->getEpollFd(), EPOLL_CTL_MOD, client.getFd(), &event);
}