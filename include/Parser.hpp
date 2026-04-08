#ifndef PARSER_HPP
#define PARSER_HPP
#include <stdexcept>
#include <string>

#include "Message.hpp"
#include "Client.hpp"
#include "Channel.hpp"

namespace Parser {
void parseToStruct(const std::string& rawMessage, Message& msg);
void extractParams(std::string line, Message& msg);
bool isValidNickname(std::string& nick);
bool isValidChannelName(std::string& channName);
bool modeGuardChecks(Channel* ch, Message& msg); //for error logs probably would have to pass Client - nickname needed
};

#endif