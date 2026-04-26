#include "Cap.hpp"

#include "Client.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "Replies.hpp"
#include "Server.hpp"

void Cap::execute(Client& client, Message& msg, Server& server) {
  (void)server;
  if (msg.params.empty()) {
    std::string reply = Replies::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }
  std::string serverName = "localhost";
  std::string prefix = ":" + serverName + " ";
  std::string replay = prefix + " CAP * " + msg.params[0] + " :\r\n";
  client.write_msg(replay);
}

Cap globalCapCmd;