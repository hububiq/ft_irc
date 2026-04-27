#ifndef JOIN_GATEKEEPER_HPP
#define JOIN_GATEKEEPER_HPP

#include <string>

#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "reply_factory.hpp"
class JoinGatekeeper
{
  public:
	bool isJoinDenied(Channel *ch, Message &msg, Client &client);
};

#endif
