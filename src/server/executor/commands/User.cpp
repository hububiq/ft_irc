#include "User.hpp"

void User::execute(Client& client, Message& msg) {
  if (msg.params.size() < 2) {
    std::string reply = reply_factory::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }
  std::string username = msg.params[0];
  std::string realname = msg.params[1];
  if (std::find_if(username.begin(), username.end(), ::isalpha) ==
          username.end() ||
      username.find_first_of(std::string(" \r\n\0", 5)) != std::string::npos ||
      realname[0] != ':' ||
      realname.find_first_of(std::string("\0\r\n", 4)) != std::string::npos) {
    std::string reply = reply_factory::getReply(
        ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
    client.write_msg(reply);
    return;
  }
  client.setUsername(username);
  client.setRealName(realname.substr(1));
  client.setState(REGISTERED);
  client.setRegister();
  std::string reply =
      reply_factory::getReply(RPL_WELCOME, client.getNickname(),
                              client.getUsername(), client.getHostname());
  client.write_msg(reply);
}

User globalUserCmd;