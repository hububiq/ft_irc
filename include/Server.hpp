#ifndef SERVER_HPP
#define SERVER_HPP
#include <stdint.h>

#include <iostream>
#include <map>
#include <string>

#include "Client.hpp"
#include "Message.hpp"
#include "Parser.hpp"

class Server {
 private:
  int _socket_fd;
  int _port;
  uint32_t _host_ip;
  uint16_t _port_num;
  std::string _password;
  std::map<int, Client *> _clients;
  void parseArg(int argc, char **argv);
  void executeCommand(/*Client* client, */ const Message
                          &message);  // belongs to semantic parsing

  // now in request_handler -> bool process_message(Client& client)
  // bool                   processReceivedData(int client_fd);
 public:
  uint32_t getHostIp() const;
  uint16_t getPortNum() const;
  int getSocketFd() const;
  void setSocketFd(int fd);

  Server(int argc, char **argv);
  ~Server();
  void run();
};

#endif