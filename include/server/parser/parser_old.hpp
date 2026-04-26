#ifndef PARSER_HPP
#define PARSER_HPP
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"

class Server;
class Client;
class Channel;

namespace struct_parser {
void parseToStruct(const std::string& rawMessage, Message& msg);
}

bool isValidNickname(std::string& nick, Client& cli);
bool isValidChannelName(std::string& channName);
bool modeGuardChecks(Channel* ch, Message& msg, Client& client);
bool isUserInChannel(Server& server, Client& client,
                     const std::string& channelName);
bool isInviteeInChannel(Server& server, Message& msg,
                        const std::string& channelName);
bool isClientAdmin(Server& server, Client& client, std::string& chName);
void loopOnFlags(std::string chName, Server& server, Client& cli);

#endif
