#include "Parser.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Replies.hpp"


#include <algorithm>
#include <cctype>
#include <iostream>

void Parser::loopOnFlags(std::string chName, Server& server, Client& cli) {
  Channel* chan = server.getChannel(chName);
  if (!chan) {
    std::string reply = Replies::getReply(ERR_NOSUCHCHANNEL, cli.getNickname(), chName, "");
    cli.write_msg(reply);
    return ;
  }
  std::string flags = "+";
  std::string params = "";
  for (size_t i = 0; i < chan->getChanFlags().size(); i++) {
    flags += chan->getChanFlags()[i];
    if (chan->getChanFlags()[i] == 'k')
      params += " " + chan->getKey();
    if (chan->getChanFlags()[i] == 'l') {
      std::ostringstream oss;
      oss << chan->getLimit();
      params += " " + oss.str();
    }
  }
  std::string reply = Replies::getReply(RPL_CHANNELMODEIS, cli.getNickname(), chName, flags + params);
  cli.write_msg(reply);
}

bool Parser::isClientAdmin(Server &server, Client &client, std::string &chName)
{
  std::vector<Client *> admins = server.getChannel(chName)->getAdmins();
  std::string nick = client.getNickname();
  for (size_t i = 0; i < admins.size(); i++) {
    if (admins[i]->getNickname() == nick)
      return true;
  }
  return false;
}

bool Parser::isInviteeInChannel(Server& server, Message& msg, const std::string& channelName)
{
  Channel* ch = server.getChannel(channelName);
  if (!ch)
    return false;
  std::vector<Client *> memb = ch->getMembers();
  for (size_t i = 0; i < memb.size(); i++) {
    if (memb[i]->getNickname() == msg.params[0])
      return true;
  }
  return false;
}

bool Parser::isUserInChannel(Server& server, Client& client, const std::string& channelName)
{
    Channel* ch = server.getChannel(channelName);
    if (!ch)
      return false;
    std::vector<Client *>& memb = ch->getMembers();
    for (size_t i = 0; i < memb.size(); i++) {
        if (memb[i]->getNickname() == client.getNickname())
          return true;
    }
    return false;
}



bool Parser::findClient(std::map<int, Client>& cliMap, std::string& name) 
{
	std::map<int, Client>::iterator it;
	for (it = cliMap.begin(); it != cliMap.end(); it++)
	{
		if (it->second.getNickname() == name)
			return true;
	}
	return false;
}

bool Parser::modeGuardChecks(Channel* ch, Message& msg, Client& client) {
  if (ch->isInviteOnly() && !ch->isInvited(client.getNickname())) {
      std::string reply = Replies::getReply(ERR_INVITEONLYCHAN, client.getNickname(), msg.params[0], "");
      client.write_msg(reply);
      return true;
    }

  if (ch->isChannelKey()) {
       if (msg.params.size() < 2 || msg.params[1].empty()) {
        std::string reply = Replies::getReply(ERR_BADCHANNELKEY, client.getNickname(), msg.params[0], "");
        client.write_msg(reply);
        return true;
      }
      if (msg.params[1].empty()) {
        std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
        client.write_msg(reply);
        return true;
      }
      else if (msg.params[1] != ch->getKey()) {
        std::string reply = Replies::getReply(ERR_BADCHANNELKEY, client.getNickname(), msg.params[0], "");
        client.write_msg(reply);
        return true;
      }
  }
  if (ch->isChannelLimit()) {
      if (ch->getMembers().size() == ch->getLimit()) {
        std::string reply = Replies::getReply(ERR_CHANNELISFULL, client.getNickname(), msg.params[0], "");
        client.write_msg(reply);
        return true;
      }
  }
  return false;
}

bool Parser::isValidChannelName(std::string &channName)
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

bool Parser::isValidNickname(std::string& nick, Client& cli) {
  std::string specials = "-\\[]`^{}";
  if (nick.size() > 9 || !isalpha(nick[0])) {
    std::string reply = Replies::getReply(ERR_ERRONEUSNICKNAME, "*", nick, "");
    cli.write_msg(reply);
    return false;
  }
  for (size_t i = 1; i < nick.size(); i++) {
    if (!isalpha(nick[i]) && !isdigit(nick[i]) && specials.find(nick[i]) == std::string::npos) {
      std::string reply = Replies::getReply(ERR_ERRONEUSNICKNAME, "*", nick, "");
      cli.write_msg(reply);
      return false;
    }
  }
  return true;
}

void Parser::extractParams(std::string line, Message &msg)
{
	std::istringstream ss(line);
	std::string        token;

	while (ss >> token)
	{
		if (token[0] == ':')
		{
			std::string rest;
			std::getline(ss, rest);
			msg.params.push_back(token.substr(0) + rest);
			break;
		}
		else
			msg.params.push_back(token);
	}
}

void Parser::parseToStruct(const std::string& rawMessage, Message& msg) {
  std::string line = rawMessage;  
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