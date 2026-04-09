#include "Parser.hpp"

#include <algorithm>
#include <sstream>
#include <cctype>
#include <iostream>

bool Parser::modeGuardChecks(Channel* ch, Message& msg) {
  if (ch->isInviteOnly()) {
      std::cerr << "ERR_INVITEONLYCHAN - log" << std::endl;
      return true;
    }
  if (ch->isChannelKey()) {
       if (msg.params.size() < 2 || msg.params[1].empty()) {
          std::cerr << "ERR_BADCHANNELKEY - log" << std::endl;
          return true;
      }
      if (msg.params[1].empty()) {
          std::cerr << "ERR_NEEDMOREPARAMS - log" << std::endl;
          return true;
      }
      else if (msg.params[1] != ch->getKey()) {
          std::cerr << "ERR_BADCHANNELKEY - log" << std::endl;
          return true;
      }
  }
  if (ch->isChannelLimit()) {
      if (ch->getMembers().size() == ch->getLimit()) {
          std::cerr << "ERR_CHANNELISFULL - log" << std::endl;
          return true;
      }
  }
  return false;
}

bool Parser::isValidChannelName(std::string& channName) {
	if (channName.size() < 2 || channName[0] != '#' || channName.size() > 200 )
    return false;
	for (size_t i = 1; i < channName.size(); i++)
	{
		char c = channName[i];
		if (c == ' ' || c == '\0' || c == '\r' || c == '\n' || c == ',')
      return false;
	}
  return true;
}

bool Parser::isValidNickname(std::string& nick) {
  std::string specials = "-\\[]`^{}";
  if (nick.size() > 9 || !isalpha(nick[0])) {
    std::cerr << "ERR_ERRONEUSNICKNAME - log" << std::endl;
    return false;
  }
  for (size_t i = 1; i < nick.size(); i++) {
    if (!isalpha(nick[i]) && !isdigit(nick[i]) && specials.find(nick[i]) == std::string::npos) {
      std::cerr << "ERR_ERRONEUSNICKNAME - log" << std::endl;
      return false;
    }
  }
  return true;
}

void Parser::extractParams(std::string line, Message& msg) {
  std::istringstream ss(line);
  std::string token;

  while (ss >> token) {
    if (token[0] == ':') {
      std::string rest;
      std::getline(ss, rest);
      msg.params.push_back(token.substr(0) + rest);
      break;
    } else
      msg.params.push_back(token);
  }
}

void Parser::parseToStruct(const std::string& rawMessage, Message& msg) {
  std::string line = rawMessage;  // there are 510 characters maximum allowed by
                                  // protocol - should I handle it?	
  if (line[0] == ':') {
    size_t prefixEnd = line.find(' ');
    if (prefixEnd == std::string::npos)
      throw std::invalid_argument("Error: Malformed message.");
    msg.prefix = line.substr(1, prefixEnd - 1);
    line.erase(0, prefixEnd + 1);
  }
  size_t firstChar = line.find_first_not_of(" \t");
  if (firstChar != std::string::npos)
    line.erase(0, firstChar);
  else
    throw std::invalid_argument("Error: Line was empty.");
  size_t commandEnd = line.find(' ');
  if (commandEnd == std::string::npos) {
    std::transform(line.begin(), line.end(), line.begin(), ::toupper);
    msg.command = line;
    line.clear();
  } else {
    msg.command = line.substr(0, commandEnd);
    std::transform(msg.command.begin(), msg.command.end(), msg.command.begin(),
                   ::toupper);
    line.erase(0, commandEnd + 1);
    Parser::extractParams(line, msg);
  }
}
