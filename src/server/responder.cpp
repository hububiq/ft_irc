#include "server.hpp"

HandleResult responder::respond(Client& client) {
  if (client.getResponseBuffer().empty()) {
    return KEEP_CONNECTION;
  }

  const std::string& buf = client.getResponseBuffer();
  ssize_t sent = send(client.getFd(), buf.c_str(), buf.size(), 0);

  if (sent <= 0) {
    return DROP_CONNECTION;
  }

  client.getResponseBuffer().erase(0, sent);
  return KEEP_CONNECTION;
}