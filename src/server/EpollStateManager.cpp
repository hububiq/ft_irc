#include "EpollStateManager.hpp"

EpollStateManager::EpollStateManager(ServerDao *server) : m_server(server) {}

void EpollStateManager::schedule_epollout(Client &client) {
  struct epoll_event event = {};
  event.events = EPOLLIN | EPOLLOUT;
  event.data.fd = client.getFd();
  epoll_ctl(m_server->getEpollFd(), EPOLL_CTL_MOD, client.getFd(), &event);
}

void EpollStateManager::schedule_send() {
  for (std::map<int, Client>::iterator it = m_server->getClients().begin();
       it != m_server->getClients().end(); ++it) {
    if (it->second.getStatus() == READY_TO_RESPOND) {
      schedule_epollout(it->second);
    }
  }
}

void EpollStateManager::schedule_epollin(Client &client) {
  struct epoll_event event = {};
  event.events = EPOLLIN;
  event.data.fd = client.getFd();
  epoll_ctl(m_server->getEpollFd(), EPOLL_CTL_MOD, client.getFd(), &event);
}