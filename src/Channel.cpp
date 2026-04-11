//#include "Channel.hpp"
//#include "Client.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"
#include <algorithm>

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
std::string& Channel::getChannelName() { return this->_channel_name; }
std::vector<Client *>& Channel::getAdmins() { return this->_admins; }
std::vector<Client *>& Channel::getMembers() { return this->_members; }
std::vector<Client *>& Channel::getInvited() { return this->_invited; }
unsigned int Channel::getLimit() const { return this->_limit; }
std::string Channel::getKey() const { return this->_key; }
unsigned int Channel::getMaxMembers() const { return this->_limit; }
void Channel::setKey(std::string& key) { _key = key; }
void Channel::setChannelName(std::string& name) { this->_channel_name = name; }
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
