#ifndef SERVER_PARSER_MODE_REPORTER_HPP
#define SERVER_PARSER_MODE_REPORTER_HPP

#include <sstream>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "reply_factory.hpp"

class Server;
class Client;
namespace mode_reporter {
void reportChannelModes(Client& client, const std::string& channelName);
}

#endif
