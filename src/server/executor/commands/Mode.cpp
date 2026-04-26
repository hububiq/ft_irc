#include "Mode.hpp"

extern Server* g_server;

void Mode::execute(Client& client, Message& msg) {
  std::string nickname = client.getNickname();
  std::string command = msg.command;
  if (msg.params.empty()) {
    std::string reply =
        reply_factory::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
    client.write_msg(reply);
    return;
  }
  if (msg.params.size() == 1) {
    if (g_server->getChannel(msg.params[0]) == NULL) {
      std::string reply = reply_factory::getReply(ERR_NOSUCHCHANNEL, nickname,
                                                  msg.params[0], "");
      client.write_msg(reply);
      return;
    }
    if (g_server->isClientAdmin(client, msg.params[0]))
      mode_reporter::reportChannelModes(client, msg.params[0]);
    else {
      std::string reply = reply_factory::getReply(ERR_CHANOPRIVSNEEDED,
                                                  nickname, msg.params[0], "");
      client.write_msg(reply);
      return;
    }
  }
  if (msg.params.size() >= 2) {
    Channel* chanForMode = g_server->getChannel(msg.params[0]);
    if (!chanForMode) {
      std::string reply = reply_factory::getReply(ERR_NOSUCHCHANNEL, nickname,
                                                  msg.params[0], "");
      client.write_msg(reply);
      return;
    }
    std::string& chName = chanForMode->getChannelName();
    if (g_server->isClientAdmin(client, chName)) {
      if (msg.params[1][0] == '+')
        chanForMode->setFlagOn(*g_server, client, msg);
      else if (msg.params[1][0] == '-')
        chanForMode->setFlagOff(client, msg);
      else {
        std::string reply = reply_factory::getReply(
            ERR_UNKNOWNMODE, nickname, std::string(1, msg.params[1][0]), "");
        client.write_msg(reply);
      }
    } else {
      std::string reply =
          reply_factory::getReply(ERR_CHANOPRIVSNEEDED, nickname, chName, "");
      client.write_msg(reply);
    }
  }
}

Mode globalModeCmd;