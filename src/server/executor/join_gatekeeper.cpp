#include "join_gatekeeper.hpp"

bool JoinGatekeeper::isJoinDenied(Channel* ch, Message& msg, Client& client) {
  if (ch->isInviteOnly() && !ch->isInvited(client.getNickname())) {
    std::string reply = reply_factory::getReply(
        ERR_INVITEONLYCHAN, client.getNickname(), msg.params[0], "");
    client.write_msg(reply);
    return true;
  }

  if (ch->isChannelKey()) {
    if (msg.params.size() < 2 || msg.params[1].empty()) {
      std::string reply = reply_factory::getReply(
          ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
      client.write_msg(reply);
      return true;
    } else if (msg.params[1] != ch->getKey()) {
      std::string reply = reply_factory::getReply(
          ERR_BADCHANNELKEY, client.getNickname(), msg.params[0], "");
      client.write_msg(reply);
      return true;
    }
  }
  if (ch->isChannelLimit()) {
    if (ch->getMembers().size() == ch->getLimit()) {
      std::string reply = reply_factory::getReply(
          ERR_CHANNELISFULL, client.getNickname(), msg.params[0], "");
      client.write_msg(reply);
      return true;
    }
  }
  return false;
}