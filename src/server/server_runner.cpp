#include "server_runner.hpp"

extern volatile sig_atomic_t g_running;

Server *g_server = NULL;

namespace {
std::string parse_pwd(char *arg) {
  std::string pwd(arg ? arg : "");
  if (pwd.empty()) {
    throw std::out_of_range("Password must contain at least 1 letter");
  }
  return pwd;
}

long parse_port(char *arg) {
  if (!arg)
    throw std::invalid_argument("Port argument missing");
  char *endptr = NULL;
  long port = std::strtol(arg, &endptr, 10);
  if (*endptr != '\0' || port <= 1024 || port > 65535) {
    throw std::invalid_argument("Invalid port number");
  }
  return port;
}
}  // namespace

void runner::setup(int argc, char **argv) {
  if (argc != 3) {
    throw std::invalid_argument("Please use format: ./irc <port> <password>");
  }

  long port = parse_port(argv[1]);
  uint16_t port_num = htons((uint16_t)port);
  std::string pwd = parse_pwd(argv[2]);
  uint32_t host_ip = inet_addr("127.0.0.1");
  int socket_fd = listener::init_socket(host_ip, port_num);
  int epoll_fd;
  try {
    epoll_fd = epoll_initializer::init_epoll(socket_fd);
  } catch (...) {
    close(socket_fd);
    throw;
  }

  g_server = new Server(static_cast<int>(port), port_num, host_ip, pwd,
                        socket_fd, epoll_fd);
}

void runner::run() {
  multiplexer::loop_epoll();

  if (g_server) {
    delete g_server;
    g_server = NULL;
  }
}
