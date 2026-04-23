#include "CommandHandler.hpp"
#include "Replies.hpp"
#include "Cap.hpp"
#include "Invite.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Mode.hpp"
#include "Nick.hpp"
#include "Pass.hpp"
#include "Ping.hpp"
#include "PrivMsg.hpp"
#include "Quit.hpp"
#include "Topic.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "Server.hpp"

#include <map>
#include <iostream>
#include <cctype>
#include <algorithm>

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
// extern Quit globalQuitCmd;


std::map<std::string, ACommand*> CommandHandler::_commandsMap;

void CommandHandler::initCommands()
{
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
    //  this->_commandsMap["QUIT"] = &globalQuitCmd;
}

void CommandHandler::handleCommands(Client& client, Message& msg, Server& server)
{
    std::string cmdName = msg.command;
    
    std::map<std::string, ACommand*>::iterator it = _commandsMap.find(cmdName);
    if (it != _commandsMap.end())
        it->second->execute(client, msg, server);
    else {
        std::string reply = Replies::getReply(ERR_UNKNOWNCOMMAND, client.getNickname(), cmdName, "");
        client.write_msg(reply);
    }
}






