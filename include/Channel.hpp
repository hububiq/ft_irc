#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>
#include <vector>

class Channel
{
private:
  std::string           _channel_name;
  std::string           _topic;
  Client                *_admin;
  std::vector<Client *> _admins;
  std::vector<Client*>  _clients_list;
  /* Channel MODEs */
  bool                  _i; // Set/remove Invite-only
  bool                  _t; // Set/remove the restrictions of the TOPIC command to channel operators
  std::string           _k; // Set/remove the channel key (password)
  unsigned int          _l; // Set/remove the user limit to channel
  // Give/take channel operator privilege

public:
  Channel();
  Channel(std::string& name, std::string& key, Client *admin);
  ~Channel();

  void add_client(Client*);
  void remove_client(Client*);
  void kick(Client *client, Client *target, const std::string& reason);
  void broadcast(const std::string&);
  /* 1. Some sort of broadcast message to notify
   * all clients that someone joined (_clinets_list)*/
  /* 2. general broadcast message in the channel */

  // --- get-set
  void setKey(std::string&);
  std::string getKey() const;
  void setMaxMembers(unsigned int);
  unsigned int getMaxMembers() const;
};

#endif

