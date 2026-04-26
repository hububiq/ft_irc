#include "Join.hpp"

#include "Client.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "Replies.hpp"
#include "Server.hpp"

void Join::execute(Client& client, Message& msg, Server& server) {
  std::string nickname = client.getNickname();
  std::string command = msg.command;
  if (msg.params.empty() || msg.params[0].size() == 1) {
    std::string reply = Replies::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }
  std::string& channelName = msg.params[0];
  if (!Parser::isValidChannelName(channelName)) {
    std::string reply =
        Replies::getReply(ERR_NOSUCHCHANNEL, nickname, channelName, "");
    client.write_msg(reply);
    return;
  }
  Channel* ch = server.getChannel(channelName);
  if (ch && Parser::isUserInChannel(server, client, ch->getChannelName())) {
    return;
  }
  if (ch == NULL) {
    Channel channel = Channel();
    server.addChannel(channel, channelName);
    server.getChannel(channelName)->getAdmins().push_back(&client);
    server.getChannel(channelName)->add_client(&client);
  } else if (ch != NULL) {
    if (Parser::modeGuardChecks(ch, msg, client)) return;
    ch->add_client(&client);
    std::string joinReply = ":" + client.getNickname() + "!" +
                            client.getUsername() + "@" + client.getHostname() +
                            " JOIN " + ch->getChannelName() + "\r\n";
    ch->broadcast(client, joinReply);
    client.write_msg(joinReply);
    if (ch->isThereTopic()) {
      std::string reply =
          Replies::getReply(RPL_TOPIC, nickname, msg.params[0], ch->getTopic());
      client.write_msg(reply);
      return;
    }
    std::string reply =
        Replies::getReply(RPL_NOTOPIC, nickname, msg.params[0], "");
    client.write_msg(reply);
  }
}

Join globalJoinCmd;