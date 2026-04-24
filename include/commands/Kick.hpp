#include "ACommand.hpp"
#include <string>

class Channel;
class Client;

class Kick: public ACommand
{
    public:
        void execute(Client&, Message&, Server&);
};

bool clientToBeKICKed(Channel *channel_obj, const std::string& client_to_kick);
bool clientCanKICK(Channel *channel_obj, Client &client);