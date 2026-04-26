#include "Topic.hpp"

extern Server* g_server;

void Topic::execute(Client& client, Message& msg) {
  std::string nickname = client.getNickname();
  std::string command = msg.command;
  if (msg.params.empty()) {
    std::string reply =
        reply_factory::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
    client.write_msg(reply);
    return;
  }
  Channel* chan = g_server->getChannel(msg.params[0]);
  if (!chan) {
    std::string reply =
        reply_factory::getReply(ERR_NOSUCHCHANNEL, nickname, msg.params[0], "");
    client.write_msg(reply);
    return;
  }
  bool isUserInChan = g_server->isUserInChannel(client, chan->getChannelName());
  if (!isUserInChan) {
    std::string reply =
        reply_factory::getReply(ERR_NOTONCHANNEL, nickname, msg.params[0], "");
    client.write_msg(reply);
    return;
  }
  if (chan && isUserInChan && msg.params.size() == 1) {
    if (chan->isThereTopic()) {
      std::string reply = reply_factory::getReply(
          RPL_TOPIC, nickname, msg.params[0], chan->getTopic());
      client.write_msg(reply);
    } else {
      std::string reply =
          reply_factory::getReply(RPL_NOTOPIC, nickname, msg.params[0], "");
      client.write_msg(reply);
    }
    return;
  }
  if (isUserInChan && msg.params.size() >= 2) {
    Channel* tempChan = g_server->getChannel(msg.params[0]);
    if (!tempChan) {
      std::string reply = reply_factory::getReply(ERR_NOTONCHANNEL, nickname,
                                                  msg.params[0], "");
      client.write_msg(reply);
      return;
    }
    if (tempChan && tempChan->isTopicForOperator() &&
        !(g_server->isClientAdmin(client, msg.params[0]))) {
      std::string reply = reply_factory::getReply(ERR_CHANOPRIVSNEEDED,
                                                  nickname, msg.params[0], "");
      client.write_msg(reply);
      return;
    }
    if (tempChan && (msg.params[1].empty() || msg.params[1] == ":")) {
      tempChan->setTopic("");
      std::string reply =
          reply_factory::getReply(RPL_NOTOPIC, nickname, msg.params[0], "");
      tempChan->broadcast(client, reply);
      client.write_msg(reply);
      return;
    }
    tempChan->setTopic(msg.params[1]);
    if (!tempChan->getTopic().empty()) {
      std::string reply = ":" + client.getNickname() + "!" +
                          client.getUsername() + "@" + client.getHostname() +
                          " TOPIC " + tempChan->getChannelName() + " " +
                          tempChan->getTopic() + "\r\n";
      tempChan->broadcast(client, reply);
      client.write_msg(reply);
    }
  }
}

Topic globalTopicCmd;