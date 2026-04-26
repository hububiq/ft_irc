#include "Join.hpp"

extern Server* g_server;

void Join::execute(Client& client, Message& msg) {
  std::string nickname = client.getNickname();
  std::string command = msg.command;
  if (msg.params.empty() || msg.params[0].size() == 1) {
    std::string reply = reply_factory::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }
  std::string& channelName = msg.params[0];
  if (!validator::isValidChannelName(channelName)) {
    std::string reply =
        reply_factory::getReply(ERR_NOSUCHCHANNEL, nickname, channelName, "");
    client.write_msg(reply);
    return;
  }
  Channel* ch = g_server->getChannel(channelName);
  if (ch && g_server->isUserInChannel(client, ch->getChannelName())) {
    return;
  }
  if (ch == NULL) {
    Channel channel = Channel();
    g_server->addChannel(channel, channelName);
    g_server->getChannel(channelName)->getAdmins().push_back(&client);
    g_server->getChannel(channelName)->add_client(&client);
  } else if (ch != NULL) {
    if (join_gatekeeper::isJoinDenied(ch, msg, client)) return;
    ch->add_client(&client);
    std::string joinReply = ":" + client.getNickname() + "!" +
                            client.getUsername() + "@" + client.getHostname() +
                            " JOIN " + ch->getChannelName() + "\r\n";
    ch->broadcast(client, joinReply);
    client.write_msg(joinReply);
    if (ch->isThereTopic()) {
      std::string reply = reply_factory::getReply(
          RPL_TOPIC, nickname, msg.params[0], ch->getTopic());
      client.write_msg(reply);
      return;
    }
    std::string reply =
        reply_factory::getReply(RPL_NOTOPIC, nickname, msg.params[0], "");
    client.write_msg(reply);
  }
}

Join globalJoinCmd;