#ifndef SERVER_PARSER_VALIDATOR_HPP
#define SERVER_PARSER_VALIDATOR_HPP

#include <string>

class Client;

class Validator {
public:
  bool isValidNickname(std::string& nick, Client& cli);
  bool isValidChannelName(std::string& channName);
};

#endif
