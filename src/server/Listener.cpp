#include "Listener.hpp"

namespace {
int create_socket() {
  struct protoent *proto = getprotobyname("tcp");
  int protocol_num = proto ? proto->p_proto : 0;

  int socket_fd = socket(AF_INET, SOCK_STREAM, protocol_num);
  if (socket_fd == -1) {
    throw std::runtime_error("Failed to create socket.");
  }

  int o = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &o, sizeof(o)) == -1) {
    throw std::runtime_error("Failed to set socket SO_REUSEADDR.");
  }
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &o, sizeof(o)) == -1) {
    throw std::runtime_error("Failed to set socket SO_REUSEPORT.");
  }

  int flags = fcntl(socket_fd, F_GETFL);
  int status = fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
  if (status == -1) {
    throw std::runtime_error("Failed to set socket O_NONBLOCK");
  }
  return socket_fd;
}

void bind_socket(int socket_fd, uint32_t host_ip, uint16_t port_num) {
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = host_ip;
  addr.sin_port = port_num;

  if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    throw std::runtime_error("Failed to bind socket.");
  }

  struct sockaddr_in bound_addr;
  socklen_t bound_len = sizeof(bound_addr);
  if (getsockname(socket_fd, (struct sockaddr *)&bound_addr, &bound_len) != 0) {
    throw std::runtime_error("Failed to verify bind socket.");
  }
}

void start_socket(int socket_fd) {
  if (listen(socket_fd, SOMAXCONN) == -1) {
    throw std::runtime_error("Failed to listen on socket.");
  }
}
}  // namespace

int Listener::init_socket(uint32_t host_ip, uint16_t port_num) {
  int socket_fd = create_socket();
  bind_socket(socket_fd, host_ip, port_num);
  start_socket(socket_fd);
  return socket_fd;
}
