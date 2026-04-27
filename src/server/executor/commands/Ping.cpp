#include "Ping.hpp"

#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "reply_factory.hpp"

void Ping::execute(Client& client, Message& msg) {
  if (msg.params.empty()) {
    std::string reply = reply_factory::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }

  if (!msg.params.empty()) {
    std::string serverName = "localhost";
    std::string prefix = ":" + serverName + " ";
    std::string replay =
        prefix + " PONG " + serverName + " : " + msg.params[0] + "\r\n";
    client.write_msg(replay);
  }
}

Ping globalPingCmd;