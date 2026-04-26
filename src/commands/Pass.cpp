#include "Pass.hpp"

#include "Client.hpp"
#include "Message.hpp"
#include "Replies.hpp"
#include "Server.hpp"

void Pass::execute(Client& client, Message& msg, Server& server) {
  if (msg.params.empty()) {
    std::string reply = Replies::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }
  if ((msg.params[0] != server.getPassword())) {
    std::string reply = Replies::getReply(ERR_PASSWDMISMATCH, "*", "", "");
    client.write_msg(reply);
    return;
  }
  if (client.getAuthInfo()) {
    std::cout << client.getAuthInfo() << std::endl;
    std::string reply =
        Replies::getReply(ERR_ALREADYREGISTERED, client.getNickname(), "", "");
    client.write_msg(reply);
    return;
  }
  client.setAuth();
  client.setState(HANDSHAKE);
}

Pass globalPassCmd;