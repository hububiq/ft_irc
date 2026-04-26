#ifndef SERVER_PARSER_VALIDATOR_HPP
#define SERVER_PARSER_VALIDATOR_HPP

#include <string>

class Client;

namespace validator {
bool isValidNickname(std::string& nick, Client& cli);
bool isValidChannelName(std::string& channName);
}  // namespace validator

#endif
