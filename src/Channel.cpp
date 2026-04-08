//#include "Channel.hpp"
//#include "Client.hpp"
#include "../include/Channel.hpp"
#include "../include/Client.hpp"
#include <algorithm>

Channel::Channel(std::string& name, std::string& key, Client *admin)
{
  this->_channel_name = name;
  this->_k = key;
  this->_admin = admin;
  this->_l = 0;
}
Channel::~Channel() {}

void Channel::add_client(Client* client)
{
  this->_clients_list.push_back(client);
}
void Channel::remove_client(Client* client)
{
  if (client && this->_admin)
    // find new admin
    // and then remove
  if (client) {
    // find an object in a vector object list
    this->_clients_list.erase(
      std::find(_clients_list.begin(),
      _clients_list.end(),
      client));
  }
  // Check the std::remove also
}


void Channel::broadcast(const std::string& message)
{
  std::vector<Client*>::iterator it = _clients_list.begin();
  std::vector<Client*>::iterator it_end = _clients_list.end();

  while (it != it_end) 
  {
    (*it)->write_msg(message); // will send a message on a socket for each client
    ++it;
  }
}

// --- Getters and Setters
void Channel::setKey(std::string& key) { _k = key; }
std::string Channel::getKey() const { return this->_k; }

void Channel::setMaxMembers(unsigned int max) { _l = max; }
unsigned int Channel::getMaxMembers() const { return this->_l; }
