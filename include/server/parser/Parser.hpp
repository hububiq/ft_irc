#ifndef PARSER_HPP
#define PARSER_HPP
#include <map>
#include <stdexcept>
#include <string>

#include "Message.hpp"

class Server;
class Client;
class Channel;

namespace Parser {
void parseToStruct(const std::string& rawMessage, Message& msg);
void extractParams(std::string line, Message& msg);
bool isValidNickname(std::string& nick, Client& cli);
bool isValidChannelName(std::string& channName);
bool modeGuardChecks(Channel* ch, Message& msg, Client& client);
bool findClient(std::map<int, Client>& cliMap, std::string& name);
bool isUserInChannel(Server& server, Client& client,
                     const std::string& channelName);
bool isInviteeInChannel(Server& server, Message& msg,
                        const std::string& channelName);
bool isClientAdmin(Server& server, Client& client, std::string& chName);
void loopOnFlags(std::string chName, Server& server, Client& cli);
};  // namespace Parser

#endif
