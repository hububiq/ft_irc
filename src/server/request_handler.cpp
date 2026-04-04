#include "server.hpp"

namespace {

bool process_message(Client& client) {
  size_t end_pos = client.getRequestBuffer().find(READ_END);
  if (end_pos != std::string::npos) {
    std::string message_line = client.getRequestBuffer().substr(0, end_pos);
    client.getRequestBuffer().erase(0, end_pos + READ_END.size());
    if (!message_line.empty()) {
      std::cout << "Parsed line: " << message_line << std::endl;
      // TODO: Process the message_line (Server::executeCommand)
      // Message msg;
      // Parser::parse(message_line, msg);
      // server.executeCommand(msg);
      // Message msg;
      // if (!message_line.empty())
      // {
      //     Parser::parse(message_line, msg);
      //     this->executeCommand(/*client, */msg);
      // }
      // Szymon - Response format unclear, for now it's just a string.
      // In the future create an object, set it in client, and parse it to a string response in the responder
      client.getResponseBuffer().append(":irc-server NOTICE * :*** Received your command: " + message_line + "\r\n");
    }
    client.setStatus(READY_TO_RESPOND);
    return true;
  }
  return false;
}

HandleResult read_chunk(Client& client) {
  char recv_buffer[RECV_SIZE];
  ssize_t bytes = recv(client.getFd(), recv_buffer, sizeof(recv_buffer) - 1, 0);

  if (bytes <= 0) return DROP_CONNECTION;
  client.getRequestBuffer().append(recv_buffer, bytes);
  return KEEP_CONNECTION;
}

void schedule_epollout(int epoll_fd, Client& client) {
  struct epoll_event event = {};
  event.events = EPOLLIN | EPOLLOUT;
  event.data.fd = client.getFd();
  epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client.getFd(), &event);
}

void schedule_epollin(int epoll_fd, Client& client) {
  struct epoll_event event = {};
  event.events = EPOLLIN;
  event.data.fd = client.getFd();
  epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client.getFd(), &event);
}

}

int request_handler::process_connect(int epoll_fd, int socket_fd) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);

  int client_fd = accept(socket_fd, (struct sockaddr*)&addr, &len);
  if (client_fd >= 0) {
    int flags = fcntl(client_fd, F_GETFL, 0);
    if (flags != -1) {
      fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    }
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = client_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
  }
  return client_fd;
}

HandleResult request_handler::process_request(int epoll_fd, uint32_t events,
                                              Client& client) {
  if (events & EPOLLIN) {
    if (read_chunk(client) == DROP_CONNECTION) {
      return DROP_CONNECTION;
    }
  }

  while (1) {
    switch (client.getStatus()) {
      case READING: {
        if (!process_message(client)) {
          if (events & EPOLLOUT)
            schedule_epollin(epoll_fd, client);
          return KEEP_CONNECTION;
        }
      }
       // fall through
      case READY_TO_RESPOND: {
        if (events & EPOLLOUT) {
          if (responder::respond(client) ==
              DROP_CONNECTION)
            return DROP_CONNECTION;
          if (client.getResponseBuffer().empty())
            client.reset();
          else
            return KEEP_CONNECTION;
        } else {
          schedule_epollout(epoll_fd, client);
          return KEEP_CONNECTION;
        }
      }
    }
  }
}