//#include "Channel.hpp"
//#include "Client.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"
#include "../include/Message.hpp"
#include "../include/Server.hpp"
#include "../include/Replies.hpp"
#include "../include/Parser.hpp"
#include <algorithm>
#include <cstdlib>

Channel::Channel():
  _limit(0), _i(false), _t(false),
  _k(false), _l(false) {}

Channel::~Channel() {}

void Channel::add_client(Client* cli) { ///tu teraz zmienic
  this->_members.push_back(cli);
}
void  Channel::addToInvited(std::string clientName) { 
  this->_invited.push_back(clientName); 
  //dac w sygnaturze std::string nick  i wyszukiwac po nicku w mapie klientow serwera?
}
void Channel::addToChanFlags(char flag) {
  std::vector<char>::iterator it = std::find(_chanFlags.begin(), _chanFlags.end(), flag);
  if (it == _chanFlags.end())
    this->_chanFlags.push_back(flag);
}

void Channel::addToAdmins(Server& serv, Client& cli, std::string& newAdmin) {
  std::map<int, Client>& members = serv.getClients();
  std::map<int, Client>::iterator it = members.begin();
  while (it != members.end()) {
    if (it->second.getNickname() == newAdmin) {
      this->_admins.push_back(&it->second);
      return ;
    }
    it++;
  }
  std::string reply = Replies::getReply(ERR_NOSUCHNICK, cli.getNickname(), newAdmin, "");
  cli.write_msg(reply);
}

void Channel::removeFromFlags(char flag) {
  std::vector<char>& flagVec = this->getChanFlags();
  std::vector<char>::iterator it = flagVec.begin();
  while (it != flagVec.end()) {
    if (*it == flag)
      it = flagVec.erase(it);
    else
      it++;
  }
}

std::string& Channel::getChannelName() { return this->_channel_name; }
std::vector<Client *>& Channel::getAdmins() { return this->_admins; }
std::vector<Client *>& Channel::getMembers() { return this->_members; }
std::vector<std::string>& Channel::getInvited() { return this->_invited; }
std::vector<char>& Channel::getChanFlags() { return this->_chanFlags; }
unsigned int Channel::getLimit() const { return this->_limit; }
std::string Channel::getKey() const { return this->_key; }
std::string Channel::getTopic() const { return this->_topic; }
unsigned int Channel::getMaxMembers() const { return this->_limit; }
void Channel::setKey(std::string& key) { _key = key; }
void Channel::setTopic(std::string topic) { this->_topic = topic; }
void Channel::setChannelName(std::string& name) { this->_channel_name = name; }
void Channel::setLimit(unsigned int l) { this->_limit = l; }



/*turning on the flags*/
void Channel::handleTurnL(Client& client, std::string& flagStr, int i, Message& msg) {
  if (!this->isChannelLimit() )
    this->toggleParticularFlag(this->_l);
  this->addToChanFlags(flagStr[i]);
  this->setLimit(std::atoi(msg.params[2].c_str()));
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
    + "@" + client.getHostname() + " MODE " + this->getChannelName() + " +l " + msg.params[2] + "\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
  if (msg.params.size() > 2)
    msg.params.erase(msg.params.begin() + 2);
}

void Channel::handleTurnO(Client& client, Server& serv, Message& msg) {
  this->addToAdmins(serv, client, msg.params[2]);
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
  + "@" + client.getHostname() + " MODE " + this->getChannelName() + " +o " + msg.params[2] + "\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
  if (msg.params.size() > 2)
    msg.params.erase(msg.params.begin() + 2);
}

void Channel::handleTurnK(Client& client, std::string& flagStr, int i, Message& msg) {
  this->toggleParticularFlag(this->_k);
  this->addToChanFlags(flagStr[i]);
  this->setKey(msg.params[2]);
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
  + "@" + client.getHostname() + " MODE " + this->getChannelName() + " +k " + msg.params[2] + "\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
}

void Channel::handleTurnT(Client& client, std::string& flagStr, int i) {
  this->toggleParticularFlag(this->_t);
  this->addToChanFlags(flagStr[i]);
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
        + "@" + client.getHostname() + " MODE " + this->getChannelName() + " +t\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
}

void Channel::handleTurnI(Client& client, std::string& flagStr, int i) {
  this->toggleParticularFlag(this->_i);
  this->addToChanFlags(flagStr[i]);
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
        + "@" + client.getHostname() + " MODE " + this->getChannelName() + " +i\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
}

bool Channel::hasEnoughParams(Client& client, Message& msg) {
   if ( msg.params.size() < 3 || msg.params[2].empty()) {
      std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, client.getNickname(), msg.command, "");
      client.write_msg(reply);
      return false;
   }
   return true;
}

void Channel::setFlagOn(Server& serv, Client& client, Message msg) {
  std::string flagStr = msg.params[1];
  for (size_t i = 1; i < flagStr.size(); ++i) { //doesnt delete corresponding parameter if its given to wrong flag
    if (std::string("itklo").find(flagStr[i]) == std::string::npos) {
      std::string reply = Replies::getReply(ERR_UNKNOWNMODE, client.getNickname(), std::string(1, flagStr[i]), "");
      client.write_msg(reply);
      continue;
    }
    if (flagStr[i] == 'i') {
      if (!this->isInviteOnly()) this->handleTurnI(client, flagStr, i);
    }
    else if (flagStr[i] == 't') {
      if (!this->isTopicForOperator()) this->handleTurnT(client, flagStr, i);
    }
    else if (flagStr[i] == 'k') {
      if (!this->isChannelKey()) {
        if (!hasEnoughParams(client, msg)) continue ;
        this->handleTurnK(client, flagStr, i, msg);
      }
      else {
        std::string reply = Replies::getReply(ERR_KEYSET, client.getNickname(), this->getChannelName(), "");
        client.write_msg(reply);
      }
      if (msg.params.size() > 2) msg.params.erase(msg.params.begin() + 2);
    }
    else if (flagStr[i] == 'o') {
      if (!hasEnoughParams(client, msg)) continue ;
      if (!this->isNicknameInChannel(msg.params[2])) {
        std::string reply = Replies::getReply(ERR_USERNOTINCHANNEL, client.getNickname(), msg.params[2], this->getChannelName());
        client.write_msg(reply);
        if (msg.params.size() > 2) msg.params.erase(msg.params.begin() + 2);
        continue;
      }
      this->handleTurnO(client, serv, msg);
    }
    else if (flagStr[i] == 'l') {
      if (!hasEnoughParams(client, msg)) continue;
      if (!msg.params[2].empty() && msg.params[2].find_first_not_of("0123456789") != std::string::npos) {
        if (msg.params.size() > 2) msg.params.erase(msg.params.begin() + 2);
        continue;
      }
      this->handleTurnL(client, flagStr, i, msg);
    }
  }
}
/*turning on the flags*/

/*turning off the flags*/
void Channel::handleTurnOffI(Client& client) {
  this->toggleParticularFlag(this->_i);
  this->removeFromFlags('i');
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
      + "@" + client.getHostname() + " MODE " + this->getChannelName() + " -i\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
}

void Channel::handleTurnOffT(Client& client) {
  this->toggleParticularFlag(this->_t);
  this->removeFromFlags('t');
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
      + "@" + client.getHostname() + " MODE " + this->getChannelName() + " -t\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
}

void Channel::handleTurnOffK(Client& client) {
  this->toggleParticularFlag(this->_k);
  this->removeFromFlags('k');
  this->_key.clear();
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
      + "@" + client.getHostname() + " MODE " + this->getChannelName() + " -k\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
}

void Channel::handleTurnOffL(Client& client) {
  this->toggleParticularFlag(this->_l);
  this->removeFromFlags('l');
  this->_limit = 0;
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
      + "@" + client.getHostname() + " MODE " + this->getChannelName() + " -l\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
}

void Channel::handleTurnOffO(Client& client, Message& msg) {
  std::string& targetNick = msg.params[2];
  std::vector<Client*>::iterator it;
  for (it = this->_admins.begin(); it != this->_admins.end(); it++) {
    if ((*it)->getNickname() == targetNick) {
      this->_admins.erase(it);
      break;
    }
  }
  std::string reply = ":" + client.getNickname() + "!" + client.getUsername()
      + "@" + client.getHostname() + " MODE " + this->getChannelName() + " -o " + targetNick + "\r\n";
  this->broadcast(client, reply);
  client.write_msg(reply);
  if (msg.params.size() > 2)
      msg.params.erase(msg.params.begin() + 2);
}

void Channel::setFlagOff(Client& client, Message msg) {
  std::string flagStr = msg.params[1];
  for (size_t i = 1; i < flagStr.size(); ++i) {
    if (std::string("itklo").find(flagStr[i]) == std::string::npos) {
      std::string reply = Replies::getReply(ERR_UNKNOWNMODE, client.getNickname(), std::string(1, flagStr[i]), "");
      client.write_msg(reply);
      continue;
    }
    if (flagStr[i] == 'i') {
      if (this->isInviteOnly()) this->handleTurnOffI(client);
    }
    else if (flagStr[i] == 't') {
      if (this->isTopicForOperator()) this->handleTurnOffT(client);
    }
    else if (flagStr[i] == 'k') {
      if (this->isChannelKey()) this->handleTurnOffK(client);
      if (msg.params.size() > 2) msg.params.erase(msg.params.begin() + 2);
    }
    else if (flagStr[i] == 'o') {
      if (!hasEnoughParams(client, msg)) continue;
      if (!this->isNicknameInChannel(msg.params[2])) {
          std::string reply = Replies::getReply(ERR_USERNOTINCHANNEL, client.getNickname(), msg.params[2], this->getChannelName());
          client.write_msg(reply);
          if (msg.params.size() > 2) msg.params.erase(msg.params.begin() + 2);
          continue;
      }
      this->handleTurnOffO(client, msg);
    }
    else if (flagStr[i] == 'l') {
      if (this->isChannelLimit()) this->handleTurnOffL(client);
    }
  }
}
/*turning off the flags*/

void Channel::toggleParticularFlag(bool& flag) {
  flag = !flag;
}
bool Channel::isInviteOnly() { return this->_i; }
bool Channel::isTopicForOperator() { return this->_t; }
bool Channel::isChannelKey() { return this->_k; }
bool Channel::isChannelLimit() { return this->_l; }

bool Channel::isInvited(std::string clientToFind) {
  for (size_t i = 0; i < this->getInvited().size(); i++) {
    if (this->getInvited()[i] == clientToFind)
      return true;
  }
  return false;
}

bool Channel::isThereTopic() {
  if (this->_topic.size() != 0)
    return true;
  return false;
}

bool Channel::isNicknameInChannel(std::string& nick) {
  std::vector<Client *>& members = this->getMembers();
  std::vector<Client *>::iterator it;
  for (it = members.begin(); it != members.end(); it++) {
    if ((*it)->getNickname() == nick)
      return true;
  }
  return false;
}

void Channel::broadcast(Client& sender, std::string& msg)
{
  std::vector<Client *>& members = this->getMembers();
  std::vector<Client *>::iterator it;
  for (it = members.begin(); it != members.end(); it++)
  {
    if (sender.getFd() != (*it)->getFd())
      (*it)->write_msg(msg);
  }
}

// void Channel::remove_client(Client* client)
// {
//   if (client && this->_admin)
//     // find new admin
//     // and then remove
//   if (client) {
//     // find an object in a vector object list
//     this->_clients_list.erase(
//       std::find(_clients_list.begin(),
//       _clients_list.end(), 
//       client));
//   }
//   // Check the std::remove also
// }
