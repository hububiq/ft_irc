#include "command_handler.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>

#include "../Globals.hpp"
#include "Cap.hpp"
#include "Client.hpp"
#include "Invite.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Message.hpp"
#include "Mode.hpp"
#include "Nick.hpp"
#include "Pass.hpp"
#include "Ping.hpp"
#include "PrivMsg.hpp"
#include "Quit.hpp"
#include "Replies.hpp"
#include "Server.hpp"
#include "Topic.hpp"
#include "User.hpp"

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

std::map<std::string, ACommand*> g_commandsMap;

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

void command_handler::handleCommands(Client& client, Message& msg,
                                    Server& server) {
  std::string cmdName = msg.command;

  std::map<std::string, ACommand*>::iterator it = _commandsMap.find(cmdName);
  if (it != _commandsMap.end())
    it->second->execute(client, msg, server);
  else {
    std::string reply = Replies::getReply(ERR_UNKNOWNCOMMAND,
                                          client.getNickname(), cmdName, "");
    client.write_msg(reply);
  }
}
