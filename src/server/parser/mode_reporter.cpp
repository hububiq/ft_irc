#include "mode_reporter.hpp"

extern Server* g_server;

namespace {
std::string intToString(int value) {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}


std::string buildChannelModeString(Channel& channel) {
  std::string flags = "+";
  std::string parameters = "";
  
  
  const std::vector<char>& currentFlags = channel.getChanFlags();

  for (size_t i = 0; i < currentFlags.size(); ++i) {
    char flag = currentFlags[i];
    flags += flag;

    if (flag == 'k') {
      parameters += " " + channel.getKey();
    } else if (flag == 'l') {
      parameters += " " + intToString(channel.getLimit());
    }
  }
  return flags + parameters;
}


void sendChannelReply(Client& client, ReplyCode replyCode,
                      const std::string& channelName,
                      const std::string& message) {
  std::string reply = reply_factory::getReply(replyCode, client.getNickname(),
                                              channelName, message);
  client.write_msg(reply);
}
}  

void mode_reporter::reportChannelModes(Client& client, const std::string& channelName) {
  Channel* channel = g_server->getChannel(channelName);

  if (!channel) {
    sendChannelReply(client, ERR_NOSUCHCHANNEL, channelName, "");
    return;
  }

  
  std::string modeInfo = buildChannelModeString(*channel);
  sendChannelReply(client, RPL_CHANNELMODEIS, channelName, modeInfo);
}