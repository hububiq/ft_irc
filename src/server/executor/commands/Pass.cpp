#include "Pass.hpp"

#include <iostream>
#include <string>

#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"

extern Server* g_server;

void Pass::execute(Client& client, Message& msg) {
  if (msg.params.empty()) {
    std::string reply = reply_factory::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }
  if ((msg.params[0] != g_server->getPassword())) {
    std::string reply =
        reply_factory::getReply(ERR_PASSWDMISMATCH, "*", "", "");
    client.write_msg(reply);
    return;
  }
  if (client.getAuthInfo()) {
    std::cout << client.getAuthInfo() << std::endl;
    std::string reply = reply_factory::getReply(ERR_ALREADYREGISTERED,
                                                client.getNickname(), "", "");
    client.write_msg(reply);
    return;
  }
  client.setAuth();
  client.setState(HANDSHAKE);
}

Pass globalPassCmd;