#include "Server.hpp"

#include <arpa/inet.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "./server/EpollConfig.hpp"
#include "./server/server.hpp"

Server::Server(int argc, char** argv) { parseArg(argc, argv); }

uint16_t parse_string_port(const char* port_str) {
  char* endptr;
  long port_num = std::strtol(port_str, &endptr, NUM_BASE);

  if (*endptr != '\0' || port_num <= 1024 || port_num > 65535) {
    throw std::invalid_argument("Invalid port number");
  }
  return htons((uint16_t)port_num);
}

void Server::parseArg(int argc, char** argv) {
  if (argc != 3)
    throw std::invalid_argument("Please use format: ./irc <port> <password>");
  this->_port = std::atoi(argv[1]);
  this->_port_num = parse_string_port(argv[1]);
  this->_host_ip =
      inet_addr("127.0.0.1");  // sets the default address to localhost
  this->_password = std::string(argv[2]);
  if (this->_password.empty())
    throw std::out_of_range("Password must contain of at least 1 letter");
}

/* ------------------ belongs to Hubert ------------------*/
void Server::executeCommand(/*Client* client, */ const Message& message) {
  // just testing Message struct
  std::cout << message.prefix + " ";
  std::cout << message.command << std::endl;
  for (size_t i = 0; i < message.params.size(); i++)
    std::cout << message.params[i] << std::endl;
  std::cout << "Sanity check for vector of strings size: "
            << message.params.size() << std::endl;
}

// Source:
// https://deepwiki.com/42YerevanProjects/ft_irc/3-command-processing-system
void Server::run() {
  listener::init_socket(*this);
  int epoll_fd = multiplexer::init_epoll(*this);
  multiplexer::loop_epoll(epoll_fd, *this);
}

Server::~Server() {
  // cleaning
}

uint32_t Server::getHostIp() const { return this->_host_ip; }

uint16_t Server::getPortNum() const { return this->_port_num; }

int Server::getSocketFd() const { return this->_socket_fd; }

void Server::setSocketFd(int fd) { this->_socket_fd = fd; }