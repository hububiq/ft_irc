#ifndef SERVER_HPP
#define SERVER_HPP
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cctype>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "EpollConfig.hpp"
#include "HandlerStatus.hpp"
#include "Message.hpp"
#include "Parser.hpp"

class Server {
 private:
  int _socket_fd;
  int _epoll_fd;
  int _port;
  uint32_t _host_ip;
  uint16_t _port_num;
  std::string _password;
  std::map<int, Client> _clients;
  std::map<std::string, Channel> _channels;

  uint16_t parse_string_port(const char *port_str);
  namespace listener {
  int init_socket();
  }
  namespace multiplexer {
  int init_epoll();
  void loop_epoll();
  }  // namespace multiplexer
  namespace epoll_state_manager {
  void schedule_epollin(Client &client);
  void schedule_send();
  }  // namespace epoll_state_manager
  namespace request_handler {
  HandleResult process_request(uint32_t events, Client &client);
  }
  namespace conn_handler {
  void process_connect(int socket_fd);
  }

 public:
  std::string getPassword();
  Server(int argc, char **argv);
  ~Server();
  void run();

  std::map<int, Client> &getClients();
  Channel *getChannel(const std::string &chann);
  std::map<std::string, Channel> &getChannels();
  void addChannel(Channel &ch, std::string &chName);
};

#endif
