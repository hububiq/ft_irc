#ifndef CHANNEL_HPP
#define CHANNEL_HPP

// #include "Client.hpp"

#include <string>
#include <vector>

class Client;
struct Message;

class Channel
{
private:
  std::string           _channel_name;
  std::string           _topic;
  std::vector<Client *> _admins;
  std::vector<Client *>  _members;
  std::vector<Client *> _invited;
  std::vector<char>     _chanFlags;
  std::string           _key; 
  unsigned int          _limit;
  bool                  _i;
  bool                  _t;
  bool                  _k;
  bool                  _l;
  bool                  _o;

  //implement CHANNELMODEIS and somehow print all the flags

public:
  Channel();
  ~Channel();

  void add_client(Client* cli);
  void addToInvited(Client* client);
  void addToChanFlags(char flag);
  void addToAdmins(Server& serv, Client& cli, std::string& newAdmin);
  void removeFromFlags(char flag);
  // void remove_client(Client*);
  // void remove_client(Client*);
  void kick(Client *client, Client *target, const std::string& reason);
  void broadcast(const std::string&);

  std::string&           getChannelName();
  std::vector<Client *>& getAdmins();
  std::vector<Client *>& getMembers();
  std::vector<Client *>& getInvited();
  std::vector<char>&     getChanFlags();
  unsigned int           getLimit() const;
  std::string            getKey() const;
  std::string            getTopic() const;
  unsigned int           getMaxMembers() const;
  void            setKey(std::string& key);
  void            setTopic(std::string& topic);
  void            setChannelName(std::string& name);
  void            setLimit(unsigned int l);

  void            setFlagOn(Client &client, Message msg);
  // void            setFlagOff(Client &client, Message msg);
  void            toggleParticularFlag(bool& flag);

  bool isInviteOnly();
  bool isTopicForOperator();
  bool isChannelKey();
  bool isChannelLimit();
  bool isOperatorAssignable();
  bool isInvited(Client& clientToFind);
  bool isThereTopic();
  void broadcast(Client& sender, std::string& msg);

};

#endif

