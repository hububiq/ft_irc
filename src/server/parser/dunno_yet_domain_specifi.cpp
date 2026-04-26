bool modeGuardChecks(Channel* ch, Message& msg, Client& client) {
  if (ch->isInviteOnly() && !ch->isInvited(client.getNickname())) {
    std::string reply = Replies::getReply(
        ERR_INVITEONLYCHAN, client.getNickname(), msg.params[0], "");
    client.write_msg(reply);
    return true;
  }

  if (ch->isChannelKey()) {
    if (msg.params.size() < 2 || msg.params[1].empty()) {
      std::string reply = Replies::getReply(
          ERR_BADCHANNELKEY, client.getNickname(), msg.params[0], "");
      client.write_msg(reply);
      return true;
    }
    if (msg.params[1].empty()) {
      std::string reply = Replies::getReply(
          ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
      client.write_msg(reply);
      return true;
    } else if (msg.params[1] != ch->getKey()) {
      std::string reply = Replies::getReply(
          ERR_BADCHANNELKEY, client.getNickname(), msg.params[0], "");
      client.write_msg(reply);
      return true;
    }
  }
  if (ch->isChannelLimit()) {
    if (ch->getMembers().size() == ch->getLimit()) {
      std::string reply = Replies::getReply(
          ERR_CHANNELISFULL, client.getNickname(), msg.params[0], "");
      client.write_msg(reply);
      return true;
    }
  }
  return false;
}

void loopOnFlags(std::string chName, Server& server, Client& cli) {
  Channel* chan = server.getChannel(chName);
  if (!chan) {
    std::string reply =
        Replies::getReply(ERR_NOSUCHCHANNEL, cli.getNickname(), chName, "");
    cli.write_msg(reply);
    return;
  }
  std::string flags = "+";
  std::string params = "";
  for (size_t i = 0; i < chan->getChanFlags().size(); i++) {
    flags += chan->getChanFlags()[i];
    if (chan->getChanFlags()[i] == 'k') params += " " + chan->getKey();
    if (chan->getChanFlags()[i] == 'l') {
      std::ostringstream oss;
      oss << chan->getLimit();
      params += " " + oss.str();
    }
  }
  std::string reply = Replies::getReply(RPL_CHANNELMODEIS, cli.getNickname(),
                                        chName, flags + params);
  cli.write_msg(reply);
}
