#include "Invite.hpp"

void Invite::execute(Client &client, Message &msg) {
  std::string nickname = client.getNickname();
  std::string command = msg.command;
  if (msg.params.size() < 2) {
    std::string reply =
        reply_factory::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
    client.write_msg(reply);
    return;
  }
  if (!m_server->checkIfClientExistsByNickname(msg.params[0])) {
    std::string reply =
        reply_factory::getReply(ERR_NOSUCHNICK, nickname, msg.params[1], "");
    client.write_msg(reply);
    return;
  }
  Channel *ch = m_server->getChannel(msg.params[1]);
  if (!ch) {
    std::string reply = reply_factory::getReply(ERR_NOSUCHCHANNEL, nickname,
                                                msg.params[0], msg.params[1]);
    client.write_msg(reply);
    return;
  }
  if (ch && !m_server->isUserInChannel(client, ch->getChannelName())) {
    std::string reply =
        reply_factory::getReply(ERR_NOTONCHANNEL, nickname, msg.params[1], "");
    client.write_msg(reply);
    return;
  }
  if (ch && m_server->isInviteeInChannel(msg, ch->getChannelName())) {
    std::string reply = reply_factory::getReply(ERR_USERONCHANNEL, nickname,
                                                msg.params[0], msg.params[1]);
    client.write_msg(reply);
    return;
  }
  if (!m_validator->isValidChannelName(msg.params[1])) {
    std::string reply =
        reply_factory::getReply(ERR_NOSUCHCHANNEL, nickname, msg.params[0], "");
    client.write_msg(reply);
    return;
  }
  if (ch && ch->isInviteOnly()) {
    if (!m_server->isClientAdmin(client, msg.params[1])) {
      std::string reply = reply_factory::getReply(ERR_CHANOPRIVSNEEDED,
                                                  nickname, msg.params[0], "");
      client.write_msg(reply);
      return;
    }
  }
  ch->addToInvited(msg.params[0]);
  std::string reply = reply_factory::getReply(RPL_INVITING, nickname,
                                              msg.params[0], msg.params[1]);
  client.write_msg(reply);
  std::map<int, Client> &cl = m_server->getClients();
  std::map<int, Client>::iterator it;
  std::string prefix = ":" + client.getNickname() + "!" + client.getUsername() +
                       "@" + client.getHostname();
  for (it = cl.begin(); it != cl.end(); it++) {
    if (it->second.getNickname() == msg.params[0]) {
      std::string reply = prefix + " INVITE " + it->second.getNickname() +
                          " :" + msg.params[1] + "\r\n";
      it->second.write_msg(reply);
    }
  }
}

Invite::Invite(ServerDao *server, Validator *Validator)
    : ACommand(server), m_validator(Validator) {}
