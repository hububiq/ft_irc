#include "command_handler.hpp"

extern Pass globalPassCmd;
extern Nick globalNickCmd;
extern User globalUserCmd;
extern Join globalJoinCmd;
extern PrivMsg globalPrivMsgCmd;
extern Kick globalKickCmd;
extern Invite globalInviteCmd;
extern Ping globalPingCmd;
extern Cap globalCapCmd;
extern Topic globalTopicCmd;
extern Mode globalModeCmd;
extern Quit globalQuitCmd;

std::map<std::string, ACommand*> _commandsMap;

void setupCommandsMap() {
  _commandsMap["PASS"] = &globalPassCmd;
  _commandsMap["NICK"] = &globalNickCmd;
  _commandsMap["USER"] = &globalUserCmd;
  _commandsMap["JOIN"] = &globalJoinCmd;
  _commandsMap["PRIVMSG"] = &globalPrivMsgCmd;
  _commandsMap["KICK"] = &globalKickCmd;
  _commandsMap["INVITE"] = &globalInviteCmd;
  _commandsMap["PING"] = &globalPingCmd;
  _commandsMap["CAP"] = &globalCapCmd;
  _commandsMap["TOPIC"] = &globalTopicCmd;
  _commandsMap["MODE"] = &globalModeCmd;
  _commandsMap["QUIT"] = &globalQuitCmd;
}

void command_handler::handleCommand(Client& client, Message& msg) {
  std::string cmdName = msg.command;

  std::map<std::string, ACommand*>::iterator it = _commandsMap.find(cmdName);
  if (it != _commandsMap.end())
    it->second->execute(client, msg);
  else {
    std::string reply = Replies::getReply(ERR_UNKNOWNCOMMAND,
                                          client.getNickname(), cmdName, "");
    client.write_msg(reply);
  }
}
