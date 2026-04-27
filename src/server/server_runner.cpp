#include "command_handler.hpp"
#include "executor.hpp"
#include "message_parser.hpp"
#include "listener.hpp"
#include "multiplexer.hpp"
#include "server_runner.hpp"

extern volatile sig_atomic_t g_running;

ServerDao *g_server = NULL;

namespace {
std::string parse_pwd(char *arg) {
  std::string pwd(arg ? arg : "");
  if (pwd.empty()) {
    throw std::out_of_range("Password must contain at least 1 letter");
  }
  return pwd;
}

long parse_port(char *arg) {
  if (!arg) throw std::invalid_argument("Port argument missing");
  char *endptr = NULL;
  long port = std::strtol(arg, &endptr, 10);
  if (*endptr != '\0' || port <= 1024 || port > 65535) {
    throw std::invalid_argument("Invalid port number");
  }
  return port;
}
}  // namespace

ServerRunner::ServerRunner(int argc, char **argv) {
  if (argc != 3) {
    throw std::invalid_argument("Please use format: ./irc <port> <password>");
  }

  long port = parse_port(argv[1]);
  uint16_t port_num = htons((uint16_t)port);
  std::string pwd = parse_pwd(argv[2]);
  uint32_t host_ip = inet_addr("127.0.0.1");
  int socket_fd = m_listener.init_socket(host_ip, port_num);
  int epoll_fd;
  try {
    epoll_fd = m_epollInitializer.init_epoll(socket_fd);
  } catch (...) {
    close(socket_fd);
    throw;
  }

  m_server = new ServerDao(static_cast<int>(port), port_num, host_ip, pwd,
                           socket_fd, epoll_fd);
  g_server = m_server;

  ConnHandler *connHandler = new ConnHandler(m_server);
  EpollStateManager *stateManager = new EpollStateManager(m_server);
  
  CommandHandler *commandHandler = new CommandHandler(m_server);
  Executor *executor = new Executor(commandHandler);
  MessageParser *messageParser = new MessageParser();

  RequestHandler *requestHandler = new RequestHandler(stateManager, executor, messageParser); 
  m_multiplexer = new Multiplexer(m_server, connHandler, requestHandler, stateManager);
}

ServerRunner::~ServerRunner() {
  if (m_multiplexer) delete m_multiplexer;

  if (m_server) {
    delete m_server;
    m_server = NULL;
    g_server = NULL;
  }
}

void ServerRunner::run() {
  m_multiplexer->loop_epoll();
}
