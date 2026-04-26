#ifndef SERVER_PARSER_JOIN_GATEKEEPER_HPP
#define SERVER_PARSER_JOIN_GATEKEEPER_HPP

#include "Channel.hpp"
#include "Client.hpp"
#include "Message.hpp"
namespace join_gatekeeper {
  bool isJoinDenied(Channel* ch, Message& msg, Client& client);
}

#endif
