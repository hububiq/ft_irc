#include "join_gatekeeper.hpp"

namespace {
bool isInviteOnlyBlocked(Channel* chan, const Client& client) {
  return chan->isInviteOnly() && !chan->isInvited(client.getNickname());
}

bool isKeyBlocked(Channel* chan, const Message& msg) {
  if (!chan->isChannelKey()) {
    return false;
  }
  if (msg.params.size() < 2 || msg.params[1] != chan->getKey()) {
    return true;
  }
  return false;
}

bool isLimitReached(Channel* chan) {
  return chan->isChannelLimit() &&
         (chan->getMembers().size() >= chan->getLimit());
}

int getJoinValidationError(Channel* chan, const Message& msg,
                           const Client& client, std::string& outExtraParam) {
  outExtraParam.clear();

  if (isInviteOnlyBlocked(chan, client)) {
    return ERR_INVITEONLYCHAN;
  }

  if (isKeyBlocked(chan, msg)) {
    return ERR_BADCHANNELKEY;
  }

  if (isLimitReached(chan)) {
    return ERR_CHANNELISFULL;
  }

  return 0;  // Success
}
}  // namespace

bool join_gatekeeper::isJoinDenied(Channel* ch, Message& msg, Client& client) {
  std::string extraParam;

  int errorCode = getJoinValidationError(ch, msg, client, extraParam);

  if (errorCode != 0) {
    std::string reply = reply_factory::getReply(
        static_cast<ReplyCode>(errorCode), client.getNickname(), msg.params[0],
        extraParam);
    client.write_msg(reply);
    return true;
  }

  return false;
}