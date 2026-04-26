#include "Server.hpp"

Server::Server(int argc, char **argv) : _socket_fd(-1),  _epoll_fd(-1) {
  if (argc != 3)
    throw std::invalid_argument("Please use format: ./irc <port> <password>");
  this->_port = std::atoi(argv[1]);
  this->_port_num = parse_string_port(argv[1]);
  this->_host_ip = inet_addr("127.0.0.1");
  this->_password = std::string(argv[2]);
  if (this->_password.empty())
    throw std::out_of_range("Password must contain at least 1 letter");
}

uint16_t Server::parse_string_port(const char *port_str) {
  char *endptr;
  long port_num = std::strtol(port_str, &endptr, NUM_BASE);

  if (*endptr != '\0' || port_num <= 1024 || port_num > 65535) {
    throw std::invalid_argument("Invalid port number");
  }
  return htons((uint16_t)port_num);
}

void Server::run() {
  this->_socket_fd = init_socket();
  this->_epoll_fd = init_epoll();
  loop_epoll();
}

Server::~Server() {
  if (this->_socket_fd != -1) {
    close(this->_socket_fd);
  }
  if (this->_epoll_fd != -1) {
    close(this->_epoll_fd);
  }
  for (std::map<int, Client>::iterator it = this->_clients.begin();
       it != this->_clients.end(); ++it) {
    close(it->first);
  }
  this->_clients.clear();
}

std::string Server::getPassword() { return this->_password; }
std::map<int, Client> &Server::getClients() { return this->_clients; }

Channel *Server::getChannel(const std::string &chann) {
  std::map<std::string, Channel>::iterator it = this->_channels.find(chann);
  if (it != this->_channels.end()) return &it->second;
  return NULL;
}

std::map<std::string, Channel> &Server::getChannels() {
  return this->_channels;
}

void Server::addChannel(Channel &ch, std::string &chName) {
  ch.setChannelName(chName);
  this->_channels[chName] = ch;
}