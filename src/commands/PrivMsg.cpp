#include "PrivMsg.hpp"

#include "Client.hpp"
#include "Message.hpp"
#include "Replies.hpp"
#include "Server.hpp"

void PrivMsg::execute(Client& client, Message& msg, Server& server) {
  if (msg.params.empty() || msg.params[0].empty()) {
    std::string reply = Replies::getReply(ERR_NORECIPIENT, client.getNickname(),
                                          msg.command, "");
    client.write_msg(reply);
    return;
  }
  if (msg.params.size() < 2 || msg.params[1].empty() ||
      msg.params[1][0] != ':') {
    std::string reply =
        Replies::getReply(ERR_NOTEXTTOSEND, client.getNickname(), "", "");
    client.write_msg(reply);
    return;
  }
  Channel* channForMsg = server.getChannel(msg.params[0]);
  if (channForMsg == NULL &&
      !Parser::findClient(server.getClients(), msg.params[0])) {
    std::string reply = Replies::getReply(ERR_NOSUCHNICK, client.getNickname(),
                                          msg.params[1], "");
    client.write_msg(reply);
    return;
  }
  std::string prefix = ":" + client.getNickname() + "!" + client.getUsername() +
                       "@" + client.getHostname();
  if (channForMsg != NULL) {
    std::string reply = prefix + " PRIVMSG " + channForMsg->getChannelName() +
                        " " + msg.params[1] + "\r\n";
    channForMsg->broadcast(client, reply);
  } else if (msg.params[0][0] != '#') {
    std::map<int, Client>& cl = server.getClients();
    std::map<int, Client>::iterator it;
    for (it = cl.begin(); it != cl.end(); it++) {
      if (it->second.getNickname() == msg.params[0]) {
        std::string reply = prefix + " PRIVMSG " + it->second.getNickname() +
                            " " + msg.params[1] + "\r\n";
        it->second.write_msg(reply);
      }
    }
  }
}

PrivMsg globalPrivMsgCmd;