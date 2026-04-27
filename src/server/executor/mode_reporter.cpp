#include "mode_reporter.hpp"

extern ServerDao* g_server;

namespace mode_reporter {
void reportChannelModes(Client& cli, const std::string& chName) {
  Channel* chan = g_server->getChannel(chName);
  if (!chan) {
    std::string reply = reply_factory::getReply(ERR_NOSUCHCHANNEL,
                                                cli.getNickname(), chName, "");
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
  std::string reply = reply_factory::getReply(
      RPL_CHANNELMODEIS, cli.getNickname(), chName, flags + params);
  cli.write_msg(reply);
}
}  // namespace mode_reporter