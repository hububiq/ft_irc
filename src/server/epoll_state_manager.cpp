void Server::schedule_send() {
  for (std::map<int, Client>::iterator it = this->_clients.begin();
       it != this->_clients.end(); ++it) {
    if (it->second.getStatus() == READY_TO_RESPOND) {
      schedule_epollout(it->second);
    }
  }
}

void Server::schedule_epollout(Client &client) {
  struct epoll_event event = {};
  event.events = EPOLLIN | EPOLLOUT;
  event.data.fd = client.getFd();
  epoll_ctl(this->_epoll_fd, EPOLL_CTL_MOD, client.getFd(), &event);
}

void Server::schedule_epollin(Client &client) {
  struct epoll_event event = {};
  event.events = EPOLLIN;
  event.data.fd = client.getFd();
  epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client.getFd(), &event);
}