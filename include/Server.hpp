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
#include "HandlerStatus.hpp"
#include "EpollConfig.hpp"
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
  /* Listener.cpp */
  int create_socket();
  void bind_socket(int socket_fd);
  void start_socket(int socket_fd);
  int init_socket();
  /* Multiplexer.cpp */
  int init_epoll();
  void register_socket(int epoll_fd);
  void loop_epoll();
  /* Executor.cpp */
  void executeMessage(Client &client, Message &msg);
  /* StateManager */
  void schedule_epollout(Client &client);
  void schedule_epollin(Client &client);
  void schedule_send();
  /* RequestHandler */
  HandleResult process_request(int epoll_fd, uint32_t events, Client &client);
  HandleResult respond(Client &client);
  bool process_message(Client &client);
  HandleResult read_chunk(Client &client);
  /* ConnHandler */
  void register_client(int client_fd, std::string &hostname,
                       std::map<int, Client> &client);
  void process_connect(int epoll_fd, int socket_fd,
                       std::map<int, Client> &clients);

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
