#include "Validator.hpp"

#include <cctype>

#include "Client.hpp"
#include "reply_factory.hpp"

bool Validator::isValidNickname(std::string &nick, Client &cli)
{
	std::string specials = "-\\[]`^{}";
	if (nick.size() > 9 || !isalpha(nick[0]))
	{
		std::string reply = reply_factory::getReply(ERR_ERRONEUSNICKNAME, "*", nick, "");
		cli.write_msg(reply);
		return false;
	}
	for (size_t i = 1; i < nick.size(); i++)
	{
		if (!isalpha(nick[i]) && !isdigit(nick[i]) && specials.find(nick[i]) == std::string::npos)
		{
			std::string reply = reply_factory::getReply(ERR_ERRONEUSNICKNAME, "*", nick, "");
			cli.write_msg(reply);
			return false;
		}
	}
	return true;
}

bool Validator::isValidChannelName(std::string &channName)
{
	if (channName.size() < 2 || channName[0] != '#' || channName.size() > 200)
		return false;
	for (size_t i = 1; i < channName.size(); i++)
	{
		char c = channName[i];
		if (c == ' ' || c == '\0' || c == '\r' || c == '\n' || c == ',')
			return false;
	}
	return true;
}
