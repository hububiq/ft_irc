#include "Cap.hpp"

#include "Client.hpp"
#include "Message.hpp"
#include "ServerDao.hpp"
#include "reply_factory.hpp"

void Cap::execute(Client& client, Message& msg) {
  if (msg.params.empty()) {
    std::string reply = reply_factory::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }
  std::string serverName = "localhost";
  std::string prefix = ":" + serverName + " ";
  std::string replay = prefix + " CAP * " + msg.params[0] + " :\r\n";
  client.write_msg(replay);
}







Cap::Cap(ServerDao *server) : ACommand(server) {}
