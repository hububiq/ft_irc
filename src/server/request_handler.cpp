#include "Server.hpp"

namespace {
bool process_message(Client &client) {
  size_t end_pos = client.getRequestBuffer().find(READ_END);
  if (end_pos != std::string::npos) {
    std::string messageLine = client.getRequestBuffer().substr(0, end_pos);
    client.getRequestBuffer().erase(0, end_pos + READ_END.size());
    Message msg;
    if (!messageLine.empty()) {
      Parser::parseToStruct(messageLine, msg);
      executor::executeMessage(client, msg, *this);
    }
    client.setStatus(READY_TO_RESPOND);
    return true;
  }
  return false;
}

HandleResult read_chunk(Client &client) {
  char recv_buffer[RECV_SIZE];
  ssize_t bytes = recv(client.getFd(), recv_buffer, sizeof(recv_buffer) - 1, 0);

  if (bytes <= 0) return DROP_CONNECTION;
  client.getRequestBuffer().append(recv_buffer, bytes);
  return KEEP_CONNECTION;
}

HandleResult respond(Client &client) {
  if (client.getResponseBuffer().empty()) {
    return KEEP_CONNECTION;
  }

  const std::string &buf = client.getResponseBuffer();
  ssize_t sent = send(client.getFd(), buf.c_str(), buf.size(), 0);

  if (sent < 0) {
    return DROP_CONNECTION;
  }

  client.getResponseBuffer().erase(0, sent);
  return KEEP_CONNECTION;
}
}  // namespace

HandleResult request_handler::process_request(uint32_t events, Client &client) {
  if (events & EPOLLIN && (!(events & EPOLLOUT))) {
    if (read_chunk(client) == DROP_CONNECTION) {
      return DROP_CONNECTION;
    }
  }

  while (1) {
    switch (client.getStatus()) {
      case READING: {
        if (!process_message(client)) {
          if (events & EPOLLOUT) state_manager::schedule_epollin(client);
          return KEEP_CONNECTION;
        }
      }
      // fall through
      case READY_TO_RESPOND: {
        if (events & EPOLLOUT) {
          if (respond(client) == DROP_CONNECTION) return DROP_CONNECTION;
          if (client.getResponseBuffer().empty()) {
            client.reset();
            break;
          } else
            return KEEP_CONNECTION;
        } else {
          return KEEP_CONNECTION;
        }
      }
    }
  }
}