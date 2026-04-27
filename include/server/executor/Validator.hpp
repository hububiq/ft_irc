#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <string>

class Client;

class Validator
{
  public:
	bool isValidNickname(std::string &nick, Client &cli);
	bool isValidChannelName(std::string &channName);
};

#endif
