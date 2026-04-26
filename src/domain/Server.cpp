#include "Server.hpp"

Server::Server(int port, uint16_t port_num, uint32_t host_ip,
               const std::string& password, int socket_fd, int epoll_fd)
    : _socket_fd(socket_fd),
      _epoll_fd(epoll_fd),
      _port(port),
      _host_ip(host_ip),
      _port_num(port_num),
      _password(password) {}

Server::~Server() {
  close(this->_socket_fd);
  close(this->_epoll_fd);
  for (std::map<int, Client>::iterator it = this->_clients.begin();
       it != this->_clients.end(); ++it) {
    close(it->first);
  }
  this->_clients.clear();
}

std::map<int, Client>& Server::getClients() { return this->_clients; }

std::map<std::string, Channel>& Server::getChannels() {
  return this->_channels;
}

Channel* Server::getChannel(const std::string& chann) {
  std::map<std::string, Channel>::iterator it = this->_channels.find(chann);
  if (it != this->_channels.end()) return &it->second;
  return NULL;
}

void Server::addChannel(Channel& ch, std::string& chName) {
  ch.setChannelName(chName);
  this->_channels[chName] = ch;
}

int Server::getSocketFd() const { return this->_socket_fd; }

int Server::getEpollFd() const { return this->_epoll_fd; }

int Server::getPort() const { return this->_port; }

uint32_t Server::getHostIp() const { return this->_host_ip; }

uint16_t Server::getPortNum() const { return this->_port_num; }

const std::string& Server::getPassword() const { return this->_password; }

bool Server::checkIfClientExistsByNickname(std::string& nickName) {
  std::map<int, Client>::iterator it;
  for (it = _clients.begin(); it != _clients.end(); it++) {
    if (it->second.getNickname() == nickName) return true;
  }
  return false;
}

bool Server::isClientAdmin(Client& client, const std::string& chName) {
  Channel* ch = this->getChannel(chName);
  if (!ch) return false;
  const std::vector<Client*>& admins = ch->getAdmins();
  std::string nick = client.getNickname();
  for (size_t i = 0; i < admins.size(); i++) {
    if (admins[i]->getNickname() == nick) return true;
  }
  return false;
}

bool Server::isInviteeInChannel(const Message& msg,
                                const std::string& channelName) {
  Channel* ch = this->getChannel(channelName);
  if (!ch) return false;
  if (msg.params.empty()) return false;
  const std::vector<Client*>& memb = ch->getMembers();
  for (size_t i = 0; i < memb.size(); i++) {
    if (memb[i]->getNickname() == msg.params[0]) return true;
  }
  return false;
}

bool Server::isUserInChannel(Client& client, const std::string& channelName) {
  Channel* ch = this->getChannel(channelName);
  if (!ch) return false;
  const std::vector<Client*>& memb = ch->getMembers();
  for (size_t i = 0; i < memb.size(); i++) {
    if (memb[i]->getNickname() == client.getNickname()) return true;
  }
  return false;
}
