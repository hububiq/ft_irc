#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

#include <string>
#include <vector>

class Channel
{
private:
	std::string           _channel_name;
	std::string           _key;
	std::string           _topic;
	//   Client                *_admin;
	std::vector<Client *> _admins;
	std::vector<Client *>  _members;
	std::vector<Client *> _invited;
	unsigned int          _limit;
	bool                  _i; // Set/remove Invite-only
	bool                  _t; // Set/remove the restrictions of the TOPIC command to channel operators
	bool                  _k; // Set/remove the channel key (password)
	bool                  _l; // Set/remove the user limit to channel
	bool                  _o;  // Give/take channel operator privilege

public:
	Channel();
	Channel(std::string& name, std::string& key, Client *admin);
	~Channel();

	void add_client(Client*);
	void addToInvited(Client*);

	// getters
	std::string&           	getChannelName();
	std::vector<Client *>& 	getAdmins();
	std::vector<Client *>& 	getMembers();
	std::vector<Client *>& 	getInvited();
	unsigned int           	getLimit() const;
	std::string            	getKey() const;
	std::string            	getTopic() const;
	unsigned int           	getMaxMembers() const;

	// setters
	void            		setKey(std::string&);
	void            		setTopic(std::string&);
	void            		setChannelName(std::string&);

	bool 					isInviteOnly();
	bool 					isTopicForOperator();
	bool 					isChannelKey();
	bool 					isChannelLimit();
	bool 					isOperatorAssignable();
	bool 					isInvited(Client&);
	bool 					isThereTopic();

	void 					broadcast(Client&, std::string&);

};

#endif

