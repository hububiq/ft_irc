//#include "Channel.hpp"
//#include "Client.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"
#include "../include/Message.hpp"
#include <algorithm>
#include <cstdlib>

Channel::Channel(): //PRIMITIVES dont initialise to 0 automatically
  _limit(0), _i(false), _t(false),
  _k(false), _l(false), _o(false) {}

// ------ NOTE TO MYSELF------need to use this constructor to make handleJoin cleaner!!------
// Channel::Channel(std::string& name, std::string& key, Client *admin)
// {
//   this->_channel_name = name;
//   this->_k = key;
//   this->_admin = admin;
//   this->_l = 0;
// }
Channel::~Channel() {}

void Channel::add_client(Client* cli) {
  this->_members.push_back(cli);
}
void  Channel::addToInvited(Client* client) { 
  this->_invited.push_back(client); 
}
void Channel::addToChanFlags(char flag) {
  this->chanFlags.push_back(flag);
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
std::vector<Client *>& Channel::getInvited() { return this->_invited; }
std::vector<char>& Channel::getChanFlags() { return this->_chanFlags; }
unsigned int Channel::getLimit() const { return this->_limit; }
std::string Channel::getKey() const { return this->_key; }
std::string Channel::getTopic() const { return this->_topic; }
unsigned int Channel::getMaxMembers() const { return this->_limit; }
void Channel::setKey(std::string& key) { _key = key; } //check is the password is valid should be performed? Password should normally go without spaces
void Channel::setTopic(std::string& topic) { this->_topic = topic; }
void Channel::setChannelName(std::string& name) { this->_channel_name = name; }
void Channel::setLimit(unsigned int l) { this->_limit = l; }


void Channel::setFlagOn(Server& serv, Client& client, Message msg) {
  std::string nickname = client.getNickname();
  std::string command = msg.command;
  std::string flagStr = msg.params[1];
  int i = 0;

  while (i < flagStr.size()) {
    if (std::string("itklo").find(flagStr[i]) == std::string::npos) {
      std::string reply = Replies::getReply(ERR_UNKNOWNMODE, client.getNickname(), flagStr[i], "");
      client.write_msg(reply);
      i++;
      continue;
    }
    if (flagStr[i] == 'i') {
      if (!this->isInviteOnly()) {
        this->toggleParticularFlag(this->_i);
        this->addToChanFlags(flagStr[i]);
        //broadcast to all, including sender
        //:{admin nickname}!{user}@{host} MODE {channel} +{flag} {parameter}
      }
    }
    else if (flagStr[i] == 't') {
      if (!this->isTopicForOperator()) {
        this->toggleParticularFlag(this->_t);
        this->addToChanFlags(flagStr[i]);
        //broadcast to all, including sender
        //:{admin nickname}!{user}@{host} MODE {channel} +{flag} {parameter}
      }
    }
    else if (flagStr[i] == 'k') {
      if (!this->isChannelKey()) {
        if ( msg.params.size() < 3 || msg.params[2].empty()) {
          std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
          client.write_msg(reply);
          i++;
          continue ;
        }
        this->toggleParticularFlag(this->_k);
        this->addToChanFlags(flagStr[i]);
        this->setKey(msg.params[2]);
        if (msg.params.size() > 2)
          msg.params.erase(msg.params.begin() + 2); //parameter for next flag when they are in the row are moving to the left now
        //broadcast to all, including sender
        //:{admin nickname}!{user}@{host} MODE {channel} +{flag} {parameter}
      }
    }
    else if (flagStr[i] == 'o') {
      if (!this->isOperatorAssignable()) {
        if (msg.params.size() < 3 || msg.params[2].empty()) {
          std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
          client.write_msg(reply);
          i++;
          continue ;
        }
        this->toggleParticularFlag(this->_o);
        this->addToChanFlags(flagStr[i]);
        this->addToAdmins(serv, client, msg.params[2]);
        if (msg.params.size() > 2)
          msg.params.erase(msg.params.begin() + 2);
        //broadcast to all, including sender
        //:{admin nickname}!{user}@{host} MODE {channel} +{flag} {parameter}
      }
    }
    else if (flagStr[i] == 'l') {
      if (!this->isChannelLimit()) {
        if ( msg.params.size() < 3 || msg.params[2].empty()) {
          std::string reply = Replies::getReply(ERR_NEEDMOREPARAMS, nickname, command, "");
          client.write_msg(reply);
          i++;
          continue ;
        }
        // the way to check if its a number \/ limit must be proper number.
        if (!msg.params[2].empty() && msg.params[2].find_first_not_of("0123456789") != std::string::npos) {
          i++;
          continue;
        }
        this->toggleParticularFlag(this->_l);
        this->addToChanFlags(flagStr[i]);
        this->setLimit(std::atoi(msg.params[2].c_str()));
        if (msg.params.size() > 2)
          msg.params.erase(msg.params.begin() + 2);
        //broadcast to all, including sender
        //:{admin nickname}!{user}@{host} MODE {channel} +{flag} {parameter}
      }
    }
    i++;
  }
}

// void Channel::setFlagOff(Client& client, Message msg) {
//   std::String flagStr = msg.params[1]; //segfault check was made in top level function
//   int i = 0;
//   while (i < flagStr.size()) {
//     if (flagStr[i] == "i" && !this->isInviteOnly()) {
//       this->toggleParticularFlag(this->_i);
//     //erase from vector with flags
//     //broadcast about switching off
//     }
//     else if (flagStr[i] == "t" && !this->isTopicForOperator()) {
//       this->toggleParticularFlag(this->_t);
//       //erase from vector with flags
//     //broadcast about switching off
//     }
//     else if (flagStr[i] == "k" && !this->isChannelKey()) {
//       this->toggleParticularFlag(this->_k);
//       //erase from vector with flags
//     //broadcast about switching off
//     }
//     else if (flagStr[i] == "o" && !this->isOperatorAssignable()) {
//       this->toggleParticularFlag(this->_o);
//       //erase from vector with flags
//     //broadcast about switching off
//     }
//     else if (flagStr[i] == "l" && !this->isChannelLimit()) {
//       this->toggleParticularFlag(this->_l);
//       // erase from vector with flags
//       // broadcast about switching off
//     }
//     else {
//         std::string reply = Replies::getReply(ERR_UNKNOWNMODE, client.getNickname(), flagStr[i], "");
//         client.write_msg(reply);
//     }
//     i++;
//   }
// }

void Channel::toggleParticularFlag(bool& flag) {
  flag = !flag;
}
bool Channel::isInviteOnly() { return this->_i; }
bool Channel::isTopicForOperator() { return this->_t; }
bool Channel::isChannelKey() { return this->_k; }
bool Channel::isChannelLimit() { return this->_l; }
bool Channel::isOperatorAssignable() { return this->_o; }

bool Channel::isInvited(Client& clientToFind) {
  for (size_t i = 0; i < this->getInvited().size(); i++) {
    if (this->getInvited()[i]->getNickname() == clientToFind.getNickname())
      return true;
  }
  return false;
}

bool Channel::isThereTopic() {
  if (this->_topic.size() != 0)
    return true;
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
